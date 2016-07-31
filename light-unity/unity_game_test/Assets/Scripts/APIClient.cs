using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Bson;

public class JsonError
{
    public string type;
    public string message;
    public int code;
    public string redirect;

    public JsonError() { }

    public JsonError(string type, string message)
    {
        this.type = type;
        this.message = message;
    }

    public JsonError(string type, string message, int code)
    {
        this.type = type;
        this.message = message;
        this.code = code;
    }

    public override string ToString()
    {
        return type + ": " + message + "(error " + code + ")";
    }
}

public abstract class APIRequest
{
    public string method = "POST";
    public string uri;
    public string responseText;
    public Dictionary<string, string> param = new Dictionary<string, string>();
    public JsonError error;
    // as seconds
    public float timeout = 30.0f;
    public object _target;

    public virtual string host { get { return string.Empty; } }
    public virtual bool clearOnError { get { return true; } }

	//public abstract void Parse(Hashtable h);
	public abstract void Parse(string json);

	// validate request before send to server
	// if error exist then set this.error
	public virtual bool Validate(ApiClient client)
    {
        //	if(!client.isAuthorized) {
        //	} else if(client.isExpired) {
        //	}
        return true;
    }

    public void SetError(string message)
    {
        this.error = new JsonError("Warning", message);
    }

    public abstract IEnumerator HandleResponse(ApiClient client);
    public virtual IEnumerator HandleError(ApiClient client)
    {
        yield break;
    }
}

public class ApiClient : MonoBehaviour
{
    string baseHost;

    public bool loggedIn { get; set; }

    private static ApiClient _instance;
    public static ApiClient instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = FindObjectOfType(typeof(ApiClient)) as ApiClient;
                if (_instance == null)
                {
                    _instance = new GameObject("APIClient").AddComponent<ApiClient>();
                }
            }
            return _instance;
        }
    }

    private void Awake()
    {
        DontDestroyOnLoad(transform.gameObject);
	}

    public void set_base_host(String host)
    {
        baseHost = host;
    }

    public GUIText uiText;

    private void Log(string str)
    {
        Debug.Log(str);
        if (uiText)
        {
            uiText.text += str + "\r\n";
        }
    }

    private void LogWarning(string str)
    {
        Debug.Log(str);
        if (uiText)
        {
            uiText.text += "Warn: " + str + "\r\n";
        }
    }

    private void LogError(string str)
    {
        Debug.Log(str);
        if (uiText)
        {
            uiText.text += "Err: " + str + "\r\n";
        }
    }

    public const string apiVersion = "0.0.1";

    private Dictionary<string, string> cookies_ = new Dictionary<string, string>();

    string cookie
    {
        get
        {
            List<string> values = new List<string>();
            foreach (KeyValuePair<string, string> kv in cookies_)
            {
                values.Add(kv.Value);
            }
            return String.Join(";", values.ToArray());
        }
    }

    public void ClearCookie()
    {
        if (cookies_ != null)
        {
            cookies_.Clear();
        }
    }

    public delegate void OnRecvHandler(object req, ApiClient client);
    public delegate IEnumerator OKHandler(object req, ApiClient client);
    public delegate IEnumerator ErrorHandler(object req, ApiClient client);

    public bool isPending = false;
    public bool isHandled = true;
    public bool isAuthorized
    {
        get
        {
            // check by cookie
            return true;
        }
    }

    // 공용 에러처리
    public void commonOnError(APIRequest req)
    {
    }

    #region Send

    private OnRecvHandler _onReceive;

    // Send packet
    public void SendPacket(APIRequest req, OnRecvHandler onRecvHandler, bool isLoadingIcon = false)
    {
        _onReceive = onRecvHandler;
        StartCoroutine(Send(req, OnOK, null));
    }

    private IEnumerator OnOK(object req, ApiClient client)
    {
        if (_onReceive != null)
        {
            OnRecvHandler temp = _onReceive;
            _onReceive = null;

            temp(req, client);
        }

        yield break;
    }

    public IEnumerator Send(APIRequest req, OKHandler onOK, ErrorHandler onError = null)
    {
        bool queueing = true;
        if (!req.Validate(this))
        {
            req.error = new JsonError("ValidateError", "invalid request");
        }

        // wait until previous Send()
        float fElapsed = 0.0f;

        if (queueing)
        {
            while (isPending && fElapsed < req.timeout)
            {
                yield return 0;
                fElapsed += Time.deltaTime;
            }

            isPending = true;
        }

        string url = baseHost + req.uri;

        WWW www = null;

        bool error = false;
        string errorMsg = "";
        string useragent = "";

        try
        {
			Dictionary<string, string> headers = new Dictionary<string, string>();

#if UNITY_ANDROID
            useragent = "light-elky/" + apiVersion + " gzip android";
#elif UNITY_IOS
			useragent = "light-elky/" + apiVersion + " gzip ios";
#else
			useragent = "light-elky/" + apiVersion + " gzip editor";
#endif

            headers["User-Agent"] = useragent;

            if (isAuthorized)
            {
                headers["COOKIE"] = cookie;
            }
            else
            {
                Log("  cookie: not found");
            }

			// zlib encoded
			string json = JsonConvert.SerializeObject(req.param);
			byte[] compressed = Ionic.Zlib.GZipStream.CompressString(json);
			//headers["Accept-Encoding"] = "gzip";
			headers["Content-Type"] = "application/json";
			headers["Content-Encoding"] = "gzip";
			www = new WWW(url, compressed, headers); // POST only
			var strBase64 = System.Convert.ToBase64String(compressed);
			Log(strBase64);

            www.threadPriority = ThreadPriority.High;

            Log("  http request sent...");
        }
        catch (System.Exception ex)
        {
            error = true;
            errorMsg = ex.ToString();
            LogWarning(errorMsg);
            LogError(req.uri + ":" + errorMsg);
        }

        // wait for http request 
        if (!error)
        {
            while (!www.isDone && www.error == null && fElapsed < req.timeout)
            {
                yield return 0;
                fElapsed += Time.deltaTime;
            }
        }

        if (queueing)
        {
            isPending = false; 
            isHandled = false;
        }

        if (error)
        {
            // handle below
            www.Dispose();
            www = null;
        }
        else if (fElapsed >= req.timeout)
        {
            www = null;
            req.error = new JsonError("TimeoutError", fElapsed.ToString());
        }
        else if (null != www.error)
        {
            req.error = new JsonError("HttpError", www.error);
            Log(www.error);
        }
        else
        {
            try
            {
                foreach (KeyValuePair<string, string> kv in www.responseHeaders)
                {
                    Log("***** [" + kv.Key + "]" + kv.Value);
                }

                // gzip check
                string text = "";
                string contentEncoding;
                www.responseHeaders.TryGetValue("Content-Encoding", out contentEncoding);
                if (string.IsNullOrEmpty(contentEncoding))
                    www.responseHeaders.TryGetValue("CONTENT-ENCODING", out contentEncoding);

                if (contentEncoding == "gzip")
                {
#if UNITY_EDITOR_OSX || UNITY_IOS
                    text = www.text;
#else
                    Log("[gzip] " + www.size + "bytes");
					Debug.LogWarning("[gzip] " + www.size + "bytes");
					byte[] decompressed = Ionic.Zlib.GZipStream.UncompressBuffer(www.bytes);
                    text = System.Text.UTF8Encoding.UTF8.GetString(decompressed);
#endif
                }
                else if (contentEncoding == "bson")
				{
					Log("[bson] " + www.size + "bytes");
					Debug.LogWarning("[bson] " + www.size + "bytes");

					MemoryStream ms = new MemoryStream(www.bytes);
					using (BsonReader reader = new BsonReader(ms))
					{
						JsonSerializer serializer = new JsonSerializer();
						object obj = serializer.Deserialize(reader);
						text = obj.ToString();
					}
				}
				else
				{
					Debug.LogWarning("[text] " + www.size + "bytes");
					text = www.text;
                }

				// cookie check
				string setCookie; // responseHeaders only have single header, until now
                bool cookieFound = false;
                www.responseHeaders.TryGetValue("Set-Cookie", out setCookie);
                if (string.IsNullOrEmpty(setCookie))
                    www.responseHeaders.TryGetValue("SET-COOKIE", out setCookie);
                if (setCookie != null && setCookie.Length > 0)
                {
                    string rawCookie = setCookie.Split(';')[0];
                    string cookieName = rawCookie.Split('=')[0]; // not to remove " 
                    cookies_[cookieName] = rawCookie;
                    cookieFound = true;
                }
                if (cookieFound)
                {
                    foreach (KeyValuePair<string, string> kv in cookies_)
                    {
                        Log("  cookie: " + kv.Value);
                    }
                }
                else if (isAuthorized)
                {
                    Log("  cookie not found from response but authorized.");
                }
                else
                {
                    Log("  cookie not found from response.");
                }

                req.responseText = text;

                // json check
                string contentType;
                bool isJson = false;
                if (www.responseHeaders.TryGetValue("Content-Type", out contentType))
                {
                    if (contentType.Contains("json"))
                    {
                        isJson = true;
                    }
                }
                else
                {
                    if (!string.IsNullOrEmpty(text)
                       && (text[0] == '{' && text[text.Length - 1] == '}'
                        || text[0] == '[' && text[text.Length - 1] == ']'))
                    {
                        isJson = true;
                    }
                }

                if (isJson)
                {
                    Log("json response. try to parse json...");
                    try
                    {
                        Log(text);
                        {
                            // try parse Response
							req.Parse(text);
                        }
                    }
                    catch (Exception ex)
                    {
                        req.error = new JsonError("ParseError", ex.ToString());
                        Debug.LogError(ex);
                    }
                }
                else
                {
                    Log("non-json response. skip parsing...");
                }
            }
            catch (Exception ex)
            {
                error = true;
                errorMsg = ex.ToString();
                LogWarning(errorMsg);
                Debug.LogError(ex);
            }
        }

        if (error)
        {
            req.error = new JsonError("ExceptionError", errorMsg);
        }

        if (req.error == null)
        {
            yield return StartCoroutine(req.HandleResponse(this));
            if (req.error == null && onOK != null)
            { // HandleResponse can throw error!

                yield return StartCoroutine(onOK(req, this));
            }
        }

        if (req.error != null)
        { // OK handler can throw error.

            yield return StartCoroutine(req.HandleError(this));

            if (onError != null)
            {
                yield return StartCoroutine(onError(req, this));
            }
            commonOnError(req);
            yield return 0;
        }

        if (queueing)
        {
            isHandled = true;
        }

        yield break;
    }

#endregion
}


public class Login : APIRequest
{
    public class Response
    {
        public long GUID;
        public string UserID;
        public string AccountName;
    }
    public Response response;

    public Login(string UserID, string Password, string DeviceType, string DeviceID, string DeviceInfo)
    {
        uri = "/login";

        param["UserID"] = UserID;
        param["Password"] = Password;
        param["Payload"] = "light_local_test";
    }

    public override bool Validate(ApiClient client)
    {
        client.ClearCookie();
        return true;
    }

    public override void Parse(string json)
    {
        response = JsonConvert.DeserializeObject<Response>(json);
    }

    public override IEnumerator HandleResponse(ApiClient client)
    {
        yield break;

    }
}


public class RelayStart : APIRequest
{
    public class member_info
    {
        public string id;
        public string hash;
    }

    public class Response
    {
        public string server_address;
        public string roomnum;
        public List<member_info> members;
    }

    public Response response;
    
    public RelayStart(int guid)
    {
        uri = "/relay/start";

        param["UserID"] = guid.ToString();
        param["RoomNumber"] = "6464";
    }

    public override void Parse(string json)
    {
        response = JsonConvert.DeserializeObject<Response>(json);
    }

    public override IEnumerator HandleResponse(ApiClient client)
    {
        yield break;

    }
}

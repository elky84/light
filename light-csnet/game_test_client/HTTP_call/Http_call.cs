using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;
using System;
using RestSharp;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

public class Http_call
{
    public delegate void delegate_callback(JObject callback);
    static RestClient client;

    public static void login(string web_address, string user_id, string password, delegate_callback callback, bool sync = false)
    {
        if(client == null)
        {
            client = new RestClient(web_address);
            client.CookieContainer = new System.Net.CookieContainer();
        }

        var request = new RestRequest("login", Method.POST);
        request.AddParameter("UserID", user_id); 
        request.AddParameter("Password", password); 
        request.AddParameter("Payload", "primus_local_test"); 

        client.ExecuteAsync(request, response => {
            Console.WriteLine(response.Content);
            if (response.Content.Length == 0)
            {
                callback(null);
                return;
            }

            JObject obj = JObject.Parse(response.Content);
            callback(obj);
        });

    }

    public static void relay_start(int guid, delegate_callback callback, bool sync = false)
    {
        if(client == null)
        {
            callback(null);
            return;
        }

        var request = new RestRequest("relay/start", Method.POST);
        request.AddParameter("RoomNumber", "6464"); 
        request.AddParameter("UserID", guid.ToString()); 

        client.ExecuteAsync(request, response => {
            Console.WriteLine(response.Content);
            if (response.Content.Length == 0)
            {
                callback(null);
                return;
            }

            JObject obj = JObject.Parse(response.Content);
            callback(obj);
        });

    }
}


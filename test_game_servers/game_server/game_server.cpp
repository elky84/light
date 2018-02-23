#include "stdafx.h"
#include "game_server.h"

#include "Listener.h"
#include "GuiPrompt.h"
#include "CommandLineArguments.h"

#include "channel_manager.h"

#include <boost/bind.hpp>

game_server::game_server(const std::wstring& program_name, const std::wstring& arg) 
	: EnvironmentManager(program_name, arg)
{
}

game_server::~game_server()
{
}

static const std::string pve_channelName = "relay:channel:pve";
static const std::string req_pve_list_channelName = "relay:channel:req_pve_list";
static const std::string res_pve_list_channelName = "relay:channel:res_pve_list";

void game_server::pve_subscribe_handler(const std::vector<char> &buf)
{
    std::string msg(buf.begin(), buf.end());
    LOG_DEBUG(L"%S", msg.c_str());

    m_channel_manager->strand_post_job(boost::bind(&channel_manager::on_redis_event, m_channel_manager.get(), msg), m_channel_manager->channel_manager_strand_position());
}

void game_server::pve_list_subscribe_handler(const std::vector<char> &buf)
{
    std::string msg(buf.begin(), buf.end());
    LOG_DEBUG(L"%S", msg.c_str());

    std::stringstream stream;
    stream.imbue(std::locale("kor", std::locale::ctype));

    stream << WSTRING_TO_STRING(m_config.IP()) << ":" << m_config.Port();
    std::string ack = stream.str();
    LOG_INFO(L"%S", ack.c_str());

    boost::asio::ip::address address = boost::asio::ip::address::from_string(m_host.c_str());

    RedisAsyncClient* publisher = new RedisAsyncClient(m_io_service);
    publisher->asyncConnect(address, m_port, [&, publisher, address, ack, msg](bool status, const std::string &err)
    {
        if (!status)
        {
            LOG_ERROR(L"Can't connect to to redis. [%S, %d] [%S]", address.to_string().c_str(), m_port, err.c_str());
        }
        else
        {
            LOG_INFO(L"connect success redis. pve_list_publisher. [%S, %d] [%S] [%S] [%S]", address.to_string().c_str(), m_port, err.c_str(), ack.c_str(), msg.c_str());
            publisher->publish(res_pve_list_channelName + msg, ack.c_str(), [&, publisher](const RedisValue &) {
                delete publisher;
            });
        }
    });
}

bool game_server::initialize()
{
	if( false == m_config.Load() )
	{
		return false;
	}
	
	light::net::Listener::create_thread(m_config.thread_count());

	m_listener.reset(new light::net::Listener(boost::bind(&game_server::peer_allocator, this, _1)));

    m_channel_manager.reset(new channel_manager);
    m_channel_manager->start();

#ifdef _DEBUG
    INT32 peer_count = 100;
#else _DEBUG
    INT32 peer_count = 5000;
#endif // _DEBUG
    light::CommandLineArguments::has_if_assign(L"PEER_COUNT", [&](const std::wstring& value) {peer_count = _wtoi(value.c_str()); });

    if (false == m_parser.load(L"config.json"))
    {
        return false;
    }

    m_host = m_parser.get<std::string>("REDIS_HOST");
    m_port = m_parser.get<unsigned short>("REDIS_PORT");
    boost::asio::ip::address address = boost::asio::ip::address::from_string(m_host.c_str());

    m_pve_subscriber.reset(new RedisAsyncClient(m_io_service));

    m_pve_subscriber->asyncConnect(address, m_port, [&](bool status, const std::string &err)
    {
        if (!status)
        {
            LOG_ERROR(L"Can't connect to to redis. [%S, %d] [%s]", m_host.c_str(), m_port, STRING_TO_WSTRING(err));
        }
        else
        {
            LOG_INFO(L"connect success redis. pve_subscriber. [%S, %d] [%S]", m_host.c_str(), m_port, STRING_TO_WSTRING(err));
            m_pve_subscriber->subscribe(pve_channelName, boost::bind(&game_server::pve_subscribe_handler, this, _1));
        }
    });

    m_pve_list_subscriber.reset(new RedisAsyncClient(m_io_service));
    m_pve_list_subscriber->asyncConnect(address, m_port, [&](bool status, const std::string &err)
    {
        if (!status)
        {
            LOG_ERROR(L"Can't connect to to redis. [%S, %d] [%S]", m_host.c_str(), m_port, STRING_TO_WSTRING(err));
        }
        else
        {
            LOG_INFO(L"connect success redis. pve_list_subscriber. [%S, %d] [%S]", m_host.c_str(), m_port, STRING_TO_WSTRING(err));
            m_pve_list_subscriber->subscribe(req_pve_list_channelName, boost::bind(&game_server::pve_list_subscribe_handler, this, _1));
        }
    });

    if (FALSE == m_listener->start(m_config.Port(), peer_count))
    {
        stop();
        return false;
    }


    m_gui_prompt.reset(new light::GuiPrompt(boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
    m_gui_prompt->start();
    
    on_complete();

	return true;
}

void game_server::stop()
{
    m_io_service.stop();
    __super::stop();
}

void game_server::on_update()
{
	Sleep(10);

	m_listener->on_update();
    m_io_service.run();
}

void game_server::release()
{
	light::net::Listener::delete_thread();	
}

light::net::PeerEx* game_server::peer_allocator(int idx)
{
    return new client_peer(idx, m_channel_manager.get());
}
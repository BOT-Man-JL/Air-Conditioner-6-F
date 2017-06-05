
//
// Air Conditioner - Client Protocol Module
// BOT Man, 2017
//

#ifndef AC_CLIENT_PROTOCOL_H
#define AC_CLIENT_PROTOCOL_H

#include <utility>
#include <memory>

#include "../common/bot-cs.h"
#include "../common/common-protocol.h"

namespace Air_Conditioner
{
    class ProtocolClient : protected BOT_CS::Client
    {
    protected:
        ProtocolClient (const std::string &ipAddr,
                        unsigned short port)
            : BOT_CS::Client (ipAddr, port) {}

        static std::unique_ptr<ProtocolClient> &_Instance ()
        {
            static std::unique_ptr<ProtocolClient> client;
            return client;
        }

    public:
        static void Init (const std::string &ipAddr,
                          unsigned short port)
        {
            _Instance () = std::unique_ptr<ProtocolClient> (
                new ProtocolClient { ipAddr, port });
        }

        static ProtocolClient &Instance ()
        {
            if (!_Instance ()) throw std::runtime_error (
                "Protocol Client has not been init");
            return *_Instance ();
        }

        void Auth (const GuestInfo &guest)
        {
            Request (AUTH, Protocol::GuestInfoToJson (guest));
        }

        std::pair<ClientInfo, ServerInfo> Req (const RoomRequest &request)
        {
            auto ret = Request (REQUEST, Protocol::RoomRequestToJson (request));
            return std::make_pair (Protocol::JsonToClientInfo (ret),
                                   Protocol::JsonToServerInfo (ret));
        }

        std::pair<ClientInfo, ServerInfo> Pulse (const RoomInfo &info)
        {
            auto ret = Request (PULSE, Protocol::RoomInfoToJson (info));
            return std::make_pair (Protocol::JsonToClientInfo (ret),
                                   Protocol::JsonToServerInfo (ret));
        }
    };
}

#endif AC_CLIENT_PROTOCOL_H
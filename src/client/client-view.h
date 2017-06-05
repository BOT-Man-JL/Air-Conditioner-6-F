
//
// Air Conditioner - Client MVC View
// BOT Man, 2017
//

#ifndef AC_CLIENT_VIEW_H
#define AC_CLIENT_VIEW_H

#include "client-model.h"
#include "../common/common-view.h"

namespace Air_Conditioner
{
    class AuthView : public ViewBase
    {
    public:
        using OnAuth = std::function<void (const GuestInfo &)>;
    };

    class ControlView : public ViewBase
    {
    public:
        using ResponseFmt = std::pair<ClientInfo, ServerInfo>;
        using OnRequest = std::function<
             ResponseFmt (const RoomRequest &)>;
        using OnPulse = std::function<
             ResponseFmt (const RoomInfo &)>;
        using OnSim = std::function<
            void (RoomInfo &, const RoomRequest &, const bool)>;
    };

    class ClientViewManager : public ViewManager
    {
    public:
        // Implemented in 'client-view-manager-*.cpp'
        void ToAuthView ();
        void ToControlView (const GuestInfo &guestInfo);
    };
}

#endif AC_CLIENT_VIEW_H
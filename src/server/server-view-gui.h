﻿
//
// Air Conditioner - Server MVC View (GUI View)
// Youjie Zhang, 2017
//

#ifndef AC_SERVER_VIEW_GUI_H
#define AC_SERVER_VIEW_GUI_H

#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>

#include <QApplication>
#include "server-view-gui-qt.h"

namespace Air_Conditioner
{
    class WelcomeViewGUI : public WelcomeView
    {     
        OnNav _onNav;
    public:
        WelcomeViewGUI (OnNav &&onNav)
            : _onNav (onNav)
        {}

        virtual void Show () override
        {

            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            WelcomeWindow welcome;
            welcome.setOnQuit([&]{
                welcome.close();
                _onNav(ViewType::Quit);

            });

            welcome.setOnConfig([&]{
                welcome.close();
                _onNav(ViewType::ConfigView);
            });

            welcome.setOnLog([&]{
                welcome.close();
                _onNav(ViewType::LogView);
            });

            welcome.setOnClient([&]{
                welcome.close();
                _onNav(ViewType::ClientView);
            });

            welcome.setOnGuest([&]{
                welcome.close();
                _onNav(ViewType::GuestView);
            });

            welcome.show();
            app.exec();

        }
    };

    class ConfigViewGUI : public ConfigView
    {
        ServerInfo _config;
        OnSet _onSet;
        OnBack _onBack;
    public:
        ConfigViewGUI (const ServerInfo &config,
                       OnSet &&onSet, OnBack &&onBack)
            : _config (config), _onSet (onSet), _onBack (onBack)
        {}

        virtual void Show () override
        {                    
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            ConfigWindow configWin;
            configWin.LoadConfig(_config);
            configWin.SetOnBack(std::move(_onBack));
            configWin.SetOnSet(std::move(_onSet));
            configWin.show();
            app.exec();
        }
    };

    class GuestViewGUI : public GuestView
    {
        GuestInfo _GetGuestInfo () const
        {
            RoomId roomId;
            GuestId guestId;

            std::cout << "Room ID: ";
            std::cin >> roomId;
            std::cout << "Guest ID: ";
            std::cin >> guestId;

            return GuestInfo {
                roomId, guestId
            };
        }

        RoomId _GetRoomId () const
        {
            RoomId roomId;

            std::cout << "Room ID: ";
            std::cin >> roomId;

            return roomId;
        }

        void _PrintList () const
        {
            if (_list.empty ())
            {
                std::cout << "No guest is registered...\n";
                return;
            }

            std::cout << "Guest on the list:\n";
            for (const auto &guest : _list)
            {
                std::cout
                    << " - Room: " << guest.room
                    << " Guest: " << guest.guest
                    << "\n";
            }
        }

        std::list<GuestInfo> _list;
        OnAdd _onAdd;
        OnDel _onDel;
        OnBack _onBack;

    public:
        GuestViewGUI (const std::list<GuestInfo> &list,
                      OnAdd &&onAdd, OnDel &&onDel,
                      OnBack &&onBack)
            : _list (list),
            _onAdd (onAdd), _onDel (onDel), _onBack (onBack)
        {}

        virtual void Show () override
        {        
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            GuestWindow guest;
            guest.SetOnBack(std::move(_onBack));
            guest.LoadGuest(_list);
            guest.SetOnAdd(std::move(_onAdd));
            guest.SetOnDel(std::move(_onDel));
            guest.SetOnBack(std::move(_onBack));
            guest.show();
            app.exec();
        }
    };

    class LogViewGUI : public LogView
    {

//        void _PrintTimeRange () const
//        {
//            std::cout << "Log starts from "
//                << TimeHelper::TimeToString (_timeBeg)
//                << " to " << TimeHelper::TimeToString (_timeEnd)
//                << std::endl;
//        }


//        TimePoint _GetTimeEnd (const TimePoint &tBeg) const
//        {
//            std::cout << "Select a Log Type (day/week/month)\n";
//            while (true)
//            {
//                auto type = InputHelper::Get<std::string> ("Log Mode");
//                if (type == "day")
//                    return tBeg + std::chrono::hours { 24 };
//                else if (type == "week")
//                    return tBeg + std::chrono::hours { 24 * 7 };
//                else if (type == "month")
//                    return tBeg + std::chrono::hours { 24 * 30 };
//                else
//                    std::cout << "Invalid Log Mode\n";
//            }
//        }

        TimePoint _timeBeg, _timeEnd;
        OnQueryOnOff _onQueryOnOff;
        OnQueryRequest _onQueryRequest;
        OnBack _onBack;

    public:
        LogViewGUI (TimePoint timeBeg,
                    TimePoint timeEnd,
                    OnQueryOnOff &&onQueryOnOff,
                    OnQueryRequest &&onQueryRequest,
                    OnBack &&onBack)
            : _timeBeg (timeBeg), _timeEnd (timeEnd),
            _onQueryOnOff (onQueryOnOff),
            _onQueryRequest (onQueryRequest),
            _onBack (onBack)
        {}

        virtual void Show () override
        {
            int tmpArgc = 0;
            char ** tmpArgv = nullptr;
            QApplication app(tmpArgc,tmpArgv);
            StatisticWindow log;
            log.show();
            log.SetOnTimeBegin([&](std::string time)->(TimePoint,TimePoint){
                    try
                    {
                        auto ret = TimeHelper::TimeFromString (str);
                        // Range Validation
                        if (ret < _timeBeg - std::chrono::hours { 24 } ||
                            ret > _timeEnd + std::chrono::hours { 24 })
                        {
                            std::cout << "Time "
                                << TimeHelper::TimeToString (time)
                                << " out of range\n";
                            _PrintTimeRange ();
                        }
                        _GetTime
                        else return ret;
                    }
                    catch (const std::exception &ex)
                    {
                        std::cout << ex.what () << std::endl;
                    }
            });

            log.SetOnExport([&](TimePoint TimeBegin,TimePoint TimeEnd){
                    _csv(TimeBegin,TimeEnd);

            });

            app.exec();
//            _PrintTimeRange ();

//            auto timeBeg = _GetTimeBeg ();
//            auto timeEnd = _GetTimeEnd (timeBeg);

//            auto onOffList = _onQueryOnOff (timeBeg, timeEnd);
//            auto requestList = _onQueryRequest (timeBeg, timeEnd);

//            // TODO: save to csv file
//            _PrintLog (onOffList, requestList);
//            std::cout << std::endl;

//            if (_onBack) _onBack ();
        }
    };

    class ClientViewGUI : public ClientView
    {
        ClientList _clients;
        OnUpdate _onUpdate;
        OnBack _onBack;

    public:
        ClientViewGUI (OnUpdate &&onUpdate, OnBack &&onBack)
            : _onUpdate (onUpdate), _onBack (onBack)
        {}

        virtual void Show () override
           {
               int tmpArgc = 0;
               char ** tmpArgv = nullptr;
               QApplication app(tmpArgc,tmpArgv);
               ClientWindow client;
               client.SetOnBack(std::move(_onBack));
               client.SetOnUpdate(std::move(_onUpdate));
               client.show();
               app.exec();
           }


    };
}

#endif AC_SERVER_VIEW_GUI_H

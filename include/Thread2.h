//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

#pragma once

#if !defined(CCLIB_THREAD2_H)
#define CCLIB_THREAD2_H

#include <predef_cc.h>

#if !defined(CC_HAVE_THREAD) || defined(CC_USE_BOOST_THREAD)
    #include <boost/thread.hpp>
    #include <boost/thread/future.hpp>
    namespace cclib
    {
        using boost::thread;
        using boost::promise;
        using boost::future;
    }

#else
    #include <thread>
    #include <future>
    namespace cclib
    {
        using std::thread;
        using std::promise;
        using std::future;
    }
#endif

#include <memory>
#include <boost/any.hpp>

namespace cclib
{
    //=======================================================================
    //==============    ThreadResult
    //=======================================================================
    class ThreadResult
    {
    private:
        boost::any      mPromise;
        boost::any      mFuture;
    public:
        void Clear()
        {
            mPromise = boost::any();
            mFuture = boost::any();
        }

        template <class T> void SetPromise( const boost::any& promise )
        {
            mPromise = promise;

            std::shared_ptr<cclib::promise<T>>    promise_ptr = boost::any_cast<std::shared_ptr<cclib::promise<T>>>(promise);
            std::shared_ptr<cclib::future<T>>     future_ptr = std::make_shared<cclib::future<T>>( promise_ptr->get_future() );

            mFuture = boost::any( future_ptr );
        }

        template <class T> std::shared_ptr<cclib::promise<T>> GetPromise()
        {
            return boost::any_cast<std::shared_ptr<cclib::promise<T>>>(mPromise);
        }

        template <class T> std::shared_ptr<cclib::future<T>> GetFuture()
        {
            return boost::any_cast<std::shared_ptr<cclib::future<T>>>(mFuture);
        }

        template <class T> bool IsClass()
        {
            //return boost::any_cast<std::shared_ptr<std::promise<T>> *>(&mPromise) != nullptr;
            bool    result = true;

            try
            {
                boost::any_cast<std::shared_ptr<cclib::future<T>>>(mFuture);
            }
            catch ( const boost::bad_any_cast& )
            {
                result = false;
            }
            return result;
        }
    };

    //=======================================================================
    //==============    Thread
    //=======================================================================
    class Thread
    {
    public:
        class Flags
        {
        private:
            std::string     mErrorMessage;
            bool            mAquireCancelRun;
            bool            mTerminated;
        public:
            Flags()
                : mErrorMessage(), mAquireCancelRun( false ), mTerminated( false )
            {}

            void CancelRun() { mAquireCancelRun = true; }
            void MarkTerminated() { mTerminated = true; }
            bool IsTerminated() const { return mTerminated; }
            bool IsCancelAquired() const { return mAquireCancelRun; }

            const std::string& GetErrorMessage() const { return mErrorMessage; }
            void SetErrorMessage( const std::string& msg ) { mErrorMessage = msg; }
        };

        typedef std::shared_ptr<Flags>      spFlags;
    private:
        typedef std::unique_ptr<cclib::thread, std::function<void( cclib::thread * )> >           uqThread;

        spFlags     mFlags;
        uqThread    mThread;
        // non-copyable
        Thread( const Thread& ) CC_EQ_DELETE;
        Thread& operator=( const Thread& ) CC_EQ_DELETE;
    public:
        Thread();
        Thread( Thread&& other );

#if ! defined (CC_HAVE_VARIADIC_TEMPLATES)
        template <class FUNC> void Run( FUNC&& func )
        {
            mFlags = std::make_shared<Flags>();
            func( mFlags );
        }

        template <class FUNC> void RunThreaded( FUNC&& func )
        {
            mFlags = std::make_shared<Flags>();
            mThread = uqThread( new cclib::thread( func, mFlags ), [this]( cclib::thread *ptr ) {
                mFlags->CancelRun();
                ptr->join();
                delete ptr;
            } );
        }
        template <class FUNC, class ARG1> void Run( FUNC&& func, ARG1&& arg1 )
        {
            mFlags = std::make_shared<Flags>();
            func( mFlags, arg1 );
        }

        template <class FUNC, class ARG1> void RunThreaded( FUNC&& func, ARG1&& arg1 )
        {
            mFlags = std::make_shared<Flags>();
            mThread = uqThread( new cclib::thread( func, mFlags, arg1 ), [this]( cclib::thread *ptr ) {
                mFlags->CancelRun();
                ptr->join();
                delete ptr;
            } );
        }
#else
        template <class FUNC, class ... Args>
        void Run( FUNC&& func, Args&& ... args )
        {
            mFlags = std::make_shared<Flags>();
            func( mFlags, args... );
        }

        template <class FUNC, class ... Args>
        void RunThreaded( FUNC&& func, Args&& ... args )
        {
            mFlags = std::make_shared<Flags>();
            mThread = uqThread( new std::thread( func, mFlags, args... ), [this]( cclib::thread *ptr ) {
                mFlags->CancelRun();
                ptr->join();
                delete ptr;
            } );
        }
#endif

        void CancelRun() { mFlags->CancelRun(); }
        void MarkTerminated() { mFlags->MarkTerminated(); }
        bool IsTerminated() const { return mFlags->IsTerminated(); }
        bool IsCancelAquired() const { return mFlags->IsCancelAquired(); }

        const std::string& GetErrorMessage() const { return mFlags->GetErrorMessage(); }
        void SetErrorMessage( const std::string& msg ) { mFlags->SetErrorMessage( msg ); }
    };

    typedef std::shared_ptr<Thread>   spThread;
    typedef std::unique_ptr<Thread>   uqThread;

}

#endif

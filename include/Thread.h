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

#if ! defined (CCLIB_THREAD_H)
#define CCLIB_THREAD_H

#include <boost/thread.hpp>
#include <boost/scope_exit.hpp>
#include <boost/shared_ptr.hpp>
#include <atomic>
#include <SharedFlag.h>

namespace cclib
{

    /*********************************************************************
    ****    ThreadBoost
    *********************************************************************/
    template <class T> class ThreadBoost
    {
    private:
        class ThreadWork
        {
        private:
            struct data_hold
            {
                T                   mWork;
                std::atomic<bool>   mRunning;

                data_hold( const T& work )
                    : mWork( work )
                {
                    mRunning = false;
                }
            };

            boost::shared_ptr<data_hold>    mWork;
        public:
            ThreadWork( const T& work )
                : mWork( new data_hold( work ) )
            {}
            void operator()()
            {
                mWork->mRunning = true;
                BOOST_SCOPE_EXIT( (&mWork) )
                {
                    mWork->mRunning = false;
                }
                BOOST_SCOPE_EXIT_END;
                try
                {
                    mWork->mWork();
                }
                catch ( const boost::thread_interrupted& )
                {
                }
                catch ( const std::exception& )
                {
                }
            }
            bool running() const { return mWork->mRunning; }
        };

        ThreadWork          mThreadWork;
        boost::thread       mThread;
        SharedFlag          mCancelF;

        // noncopyable
        ThreadBoost( const ThreadBoost& src );
        ThreadBoost& operator=( const ThreadBoost& src );
    public:
        ThreadBoost( const T& worker, const SharedFlag& cancel_f )
            : mThreadWork( worker ), mThread( mThreadWork ), mCancelF( cancel_f )
        {}
        ~ThreadBoost()
        {
            mCancelF.set_value( true );
            mThread.join();
        }
        bool running() const { return mThreadWork.running(); }
        void cancel() { mCancelF.set_value( true ); }
        bool is_canceled() { return mCancelF.is_set(); }
    };

}

#endif

#pragma once

#if ! defined (BB_THREAD_H)
#define BB_THREAD_H

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

#ifndef SDB_PROCESS_HPP
#define SDB_PROCESS_HPP

#include <filesystem>
#include <memory>
#include <sys/types.h>

namespace sdb {

    enum class process_state {
        stopped,
        running,
        exited,
        terminated
    };

    struct stop_reason {
      stop_reason(int wait_status);
      process_state reason;
      std::uint8_t info;
    };

    class process {
        public:
            // destructor is a public member
            ~process();
            static std::unique_ptr<process> launch(std::filesystem::path path);
            static std::unique_ptr<process> attach(pid_t pid);

            void resume();
            stop_reason wait_on_signal();

            // delete default constructor
            process() = delete;
            // deletes the copy constructor
            process(const process&) = delete;
            // deletes copy-assignment: you cannot assign from another
            process& operator=(const process&) = delete;

            process_state state() const { return state_; }
            pid_t pid() const { return pid_; }

        private:
            process(pid_t pid, bool terminate_on_end)
                :pid_(pid), terminate_on_end_(terminate_on_end) {}
            pid_t pid_ = 0;
            bool terminate_on_end_ = true;
            process_state state_ = process_state::stopped;
    };

}

#endif



#include "items.hh";

namespace Library {
    class User {
        private:
        std::string m_name;

        public:
        User() : m_name("Guest") {}
        User(std::String name) : m_name(name) {}


        std::string get_name() { return m_name; }

        ~User() {}
    };

    class Member : public User {
        private:

        public:
        Member(std::string name) : User(std::string name) {}

        ~Member() {}  // deconstructor for member class
    };
}
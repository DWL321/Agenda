#include <string>
class Error{
    private:
        int value;
        std::string name;
    public:
        Error();
        void setvalue(int v);
        void setname(std::string n);
        int getvalue();
        std::string getname();
        void search();
};
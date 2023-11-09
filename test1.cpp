#include <iostream>

using namespace std;

class Shader{
    public:
        Shader(string s)
        {
            this->str = s;
        }
        void get() {
            cout << this->str << endl;
        }

    private:
        string str;

};

struct pointer
{
    Shader* cell1 = new Shader("Hello!");
    Shader* cell2 = new Shader("Bye!");
};



int main()
{
    pointer shaderpointer;
    shaderpointer.cell1->get();
    shaderpointer.cell2->get();

    return 0;
}
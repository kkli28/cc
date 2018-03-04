#include <iostream>
#include <string>
using namespace std;

//========== 常量 ==========
const int BUFF_LENGTH=80;       //缓冲区大小

//========== 全局变量 ========
int lineLen=0;                  //缓冲区内的数据长度
int readPos=-1;                 //读取位置
char line[BUFF_LENGTH];         //缓冲区

//缓冲区类
class Buff{
private:
    const int BUFF_LENGTH=80;       //缓冲区大小
    
    char line[BUFF_LENGTH];         //缓冲区数组
    int lineLen=0;                  //缓冲区内的数据长度
    int readPos=-1;                 //读取位置
    int lineNum=1;                  //行号
    int colNum=0;                   //列号
    char lastCh;                    //上一个字符

//不允许手动构造、复制、赋值
private:
    Buff(std::string fileName);
    Buff(const Buff& rhs);
    const Buff& operator=(const Buff& rhs);

public:
    Buff* getInstance(std::string fileName){
        static Buff* buff=nullptr;
        if(buff==nullptr){
            buff=Buff(fileName);
        }
        return buff;
    }
    char scan();
}

//Buff构造函数
Buff::Buff(std::string fileName){
    
}
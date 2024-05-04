#include "pch.h"
#include "framework.h"
#include "afxsock.h"
#include <vector>
#include <fstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// The one and only application object

CWinApp theApp;

using namespace std;
//doc va luu thong tin anh
class PPM
{
private:
    int width = 0;
    int height = 0;
    string version = "P6";
public:
    ~PPM();
    PPM(int height, int width);
    PPM() {}
    struct RGB
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }
    void setVersion(string ver) { this->version = ver; }

    int getWidth() { return width; }
    int getHeight() { return height; }
    string getVer() { return version; }

    //void save(string name_file);
    void read(string name_file);

    void deleteImage();
    RGB** image = nullptr;

    void create_image();

};
PPM::~PPM()
{
    deleteImage();
}
PPM::PPM(int height, int width)
{
    setHeight(height);
    setWidth(width);
    create_image();
}
void PPM::read(string name_file)
{
    ifstream input(name_file, ios::binary);
    if (input.is_open())
    {
        int color;
        string ver;

        input >> ver;
        version = ver;
        input >> width;
        input >> height;
        input >> color;

        create_image();
        int box;
        if (version == "P3")
        {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                {
                    input >> box;
                    image[i][j].r = box;

                    input >> box;
                    image[i][j].g = box;

                    input >> box;
                    image[i][j].b = box;
                }
        }
        else
        {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    input.read((char*)&image[i][j], sizeof(RGB));
        }
        input.close();
    }
}
void PPM::create_image()
{
    if (image != nullptr)
    {
        deleteImage();
    }
    image = new RGB * [height];
    for (int i = 0; i < height; i++)
    {
        image[i] = new RGB[width];
        for (int j = 0; j < width; j++)
        {
            image[i][j].r = 255;
            image[i][j].g = 255;
            image[i][j].b = 255;
        }
    }
}
void PPM::deleteImage()
{
    if (image != nullptr)
    {
        for (int i = 0; i < height; i++)
            delete image[i];

        delete image;
    }
}
//doc thong tin va luu
struct PersonalInfo {
    string name;
    string phone_number;
    string email;
    string id;
    string pic="#";
};
void loadData(vector<PersonalInfo>& dtb)
{
    fstream fi;
    fi.open("thongtin.txt");
    string temp;
    PersonalInfo temp_per;

    int i(0), j(0);
    while (!fi.eof())
    {
        //
        getline(fi, temp);
        temp_per.name = temp;
        //
        getline(fi, temp);
        temp_per.phone_number = temp;
        //
        getline(fi, temp);
        temp_per.email = temp;
        //
        getline(fi, temp);
        temp_per.id = temp;
        //
        getline(fi, temp);
        temp_per.pic = temp;
        // da nhap xong 1 thanh vien 
        dtb.push_back(temp_per);
        // them thanh vien vao 
    }
    cout << "Nhap du lieu thanh cong!!!\n";
    fi.close();
}
void saveData(vector<PersonalInfo> dtb)
{
    fstream fo;
    fo.open("data.txt");
    for (int i = 0; i < dtb.size(); i++)
    {
        fo << dtb[i].name << endl;
        fo << dtb[i].phone_number << endl;
        fo << dtb[i].email << endl;
        fo << dtb[i].id << endl;
        fo << dtb[i].pic << endl;
    }
    fo.close();
}
//tim ten trong chuoi thong tin, tra lai vi tri trong chuoi thong tin hoac -1
int searchName(vector<PersonalInfo> dtb, string NAME)
{
    for (int i = 0; i < dtb.size(); i++)
    {
        if (dtb[i].name == NAME)
        {
            return i;
        }
    }
    return -1;
}
int countImage(vector<PersonalInfo> dtb)
{
    int cnt = 0;
    for (int i = 0; i < dtb.size(); i++)
    {
        if (dtb[i].pic != "#")
        {
            cnt++;
        }
    }
    return cnt;
}
//tim ten anh trong chuoi thong tin, tra lai mang ten anh
void getImage(vector<PersonalInfo> dtb, vector<string> &image_name)
{
    for (int i = 0; i < dtb.size(); i++)
    {
        if (dtb[i].pic != "#")
        {
            image_name.push_back(dtb[i].pic);
        }
    }
}
//thiet ke cho 1 luong du lieu hoat dong
DWORD WINAPI function_cal(LPVOID arg)
{
    SOCKET* hAttached = (SOCKET*)arg;
    CSocket Server;
    //chuyen ve lai CSocket
    Server.Attach(*hAttached);

    int number_continue = 1;
    int a;
    int size;
    char* name;
    vector<PersonalInfo> dtb;
    loadData(dtb);
    do {
        int a;
        int opt;
        //nhan yeu cau tu client
        Server.Receive((int*)&opt, sizeof(opt), 0);
        cout << "Da nhan duoc yeu cau nguoi dung\n";

            if (opt == 1)
            {
                //nhan ten can tim` tu client
                Server.Receive((int*)&a, sizeof(a), 0);
                name = new char[a + 1];
                Server.Receive(name, a, 0);
                name[a] = '\0';

                cout << "Da nhan duoc ten nguoi dung\n";

                // gui thong tin cua 1 nguoi
                int index = searchName(dtb, name);

                if (index == -1)// khong tim thay gui -1
                {
                    Server.Send((int*)&index, sizeof(index), 0);
                }
                else // tim thay gui vi tri va cac du lieu lien quan
                {
                    Server.Send((int*)&index, sizeof(index), 0);
                    //
                    a = dtb[index].name.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[index].name.c_str(), (int)strlen(dtb[index].name.c_str()), 0);
                    //
                    a = dtb[index].phone_number.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[index].phone_number.c_str(), (int)strlen(dtb[index].phone_number.c_str()), 0);
                    //
                    a = dtb[index].email.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[index].email.c_str(), (int)strlen(dtb[index].email.c_str()), 0);
                    //
                    a = dtb[index].id.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[index].id.c_str(), (int)strlen(dtb[index].id.c_str()), 0);
                }
                //
            }
            else if (opt == 2)
            {
                Sleep(10);

                //gui do dai mang
                size = dtb.size();
                Server.Send((int*)&size, sizeof(size), 0);
                cout << "Da gui do dai danh sach\n";
                // gui thong tin cua moi nguoi
                for (int i = 0; i < dtb.size(); i++)
                {
                    int a = dtb[i].name.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[i].name.c_str(), (int)strlen(dtb[i].name.c_str()), 0);
                    //
                    a = dtb[i].phone_number.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[i].phone_number.c_str(), (int)strlen(dtb[i].phone_number.c_str()), 0);
                    //
                    a = dtb[i].email.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[i].email.c_str(), (int)strlen(dtb[i].email.c_str()), 0);
                    //
                    a = dtb[i].id.length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(dtb[i].id.c_str(), (int)strlen(dtb[i].id.c_str()), 0);
                }
            }
            else if (opt == 3)
            {
                //nhan ten can tim` tu client
                Server.Receive((int*)&a, sizeof(a), 0);
                name = new char[a + 1];
                Server.Receive(name, a, 0);
                name[a] = '\0';
                cout << "Da nhan duoc ten nguoi dung\n";
                // 
                int index = searchName(dtb, name);

                if (index == -1)// khong tim thay gui -1
                {               
                    Server.Send((int*)&index, sizeof(index), 0);
                }
                else if (dtb[index].pic == "#")
                {
                    index = -1;
                    Server.Send((int*)&index, sizeof(index), 0);
                }
                else
                {
                    Server.Send((int*)&index, sizeof(index), 0);
                    PPM image;
                    int n;
                    unsigned char c;
                    image.read(dtb[index].pic);// luu anh thanh cong             
                    //gui phien ban
                    a = image.getVer().length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(image.getVer().c_str(), (int)strlen(image.getVer().c_str()), 0);
                    //gui HW
                    a = image.getWidth();
                    Server.Send((int*)&a, sizeof(a), 0);
                    a = image.getHeight();
                    Server.Send((int*)&a, sizeof(a), 0);
                    //gui mang 2 chieu RGB
                    for (int i = 0; i < image.getHeight(); i++)
                        for (int j = 0; j < image.getWidth(); j++)
                        {
                            c = image.image[i][j].r;
                            Server.Send(&c, sizeof(c), 0);

                            c = image.image[i][j].g;
                            Server.Send(&c, sizeof(c), 0);

                            c = image.image[i][j].b;
                            Server.Send(&c, sizeof(c), 0);

                        }

                }
            }
            else if (opt == 4)
            {
                // gui so luong anh
                size = countImage(dtb);
                Server.Send((int*)&size, sizeof(size), 0);

                //lay ten cua anh
                vector<string> image_name;
                getImage(dtb, image_name);
                for (int k = 0; k < size; k++)
                {
                    PPM image;
                    int n;
                    unsigned char c;
                    image.read(image_name[k]);// luu anh thanh cong

                    //gui phien ban
                    a = image.getVer().length();
                    Server.Send((int*)&a, sizeof(a), 0);
                    Server.Send(image.getVer().c_str(), (int)strlen(image.getVer().c_str()), 0);
                    //gui HW
                    a = image.getWidth();
                    Server.Send((int*)&a, sizeof(a), 0);
                    a = image.getHeight();
                    Server.Send((int*)&a, sizeof(a), 0);
                    //gui mang 2 chieu RGB
                    for (int i = 0; i < image.getHeight(); i++)
                        for (int j = 0; j < image.getWidth(); j++)
                        {
                            c = image.image[i][j].r;
                            Server.Send(&c, sizeof(c), 0);

                            c = image.image[i][j].g;
                            Server.Send(&c, sizeof(c), 0);

                            c = image.image[i][j].b;
                            Server.Send(&c, sizeof(c), 0);

                        }            
                }
               
            }
            else if (opt == 0)
            {
                cout << " Exit." << endl;
                number_continue = 0;
                Server.Close();
                break;
            }
    } while (number_continue);
    delete hAttached;// xoa luong
    return 0;
}
int main()
{
    int nRetCode = 0;
    HMODULE hModule = ::GetModuleHandle(nullptr);
    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            cout << "Fatal Error: MFC initialization failed\n";
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
            AfxSocketInit(NULL);
            CSocket server, s;
            DWORD threadID;
            HANDLE threadStatus;

            //tao server va port
            server.Create(4567,SOCK_STREAM,NULL);
            cout << "Waiting Client connect to ... " << endl ;
            do {
                //Server luon nghe
                server.Listen();
                server.Accept(s);
                cout << "The Client connected to The Server. " << endl;
               

                //CSocket k truyen duoc vao ham tao luong
                //->Chuyen doi CSocket thanh Socket
                //Khoi tao con tro Socket
                SOCKET* hConnected = new SOCKET();
                *hConnected = s.Detach();

                //Khoi tao thread tuong ung voi moi client Connect vao server.
                //Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
                threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);// tao luong cho 1 client
            } while (1);
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        cout << "GetModuleHandle failed\n";
        nRetCode = 1;
    }
    return nRetCode;
}

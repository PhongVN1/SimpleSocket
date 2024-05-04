#include "pch.h"
#include "framework.h"
#include "Resource.h"

#include "afxsock.h"

#include <string>
#include <vector>
#include <fstream>

int iResult;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
//doc ghi anh
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

	void save(string name_file);
	//void read(string name_file);

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
void PPM::save(string name_file)
{
	ofstream output(name_file, ios::binary);
	if (output.is_open())
	{
		output << version << endl;
		output << width << endl;
		output << height << endl;
		output << 255 << endl;
		if (version == "P3")
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					output << (int)image[i][j].r << ' ';
					output << (int)image[i][j].g << ' ';
					output << (int)image[i][j].b << '\n';
				}
		}
		else
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					output.write((char*)&image[i][j], sizeof(RGB));

			output.close();
		}
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
//doc ghi thong tin
struct PersonalInfo {
	string name;
	string phone_number;
	string email;
	string id;
};
//hien thi thong tin len console
void showInfo(vector<PersonalInfo> dtb) {
	if (dtb.size() == 0)
	{
		cout << "Empty list" << endl;
	}
	else
	{
		for (int i = 0; i < dtb.size(); i++)
		{
			cout << dtb[i].name << endl;
			cout << dtb[i].phone_number << endl;
			cout << dtb[i].email << endl;
			cout << dtb[i].id << endl;
		}
	}
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
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			// Khoi tao Thu vien
			if (AfxSocketInit() == FALSE)
			{
				cout << "Khong the khoi tao Socket Libraray";
				return FALSE;
			}

			// Tao csocket dau tien
			CSocket Client;

			// Ket noi den server
			string protocol;
			cout << " IP Address: ";
			getline(cin, protocol);

			Client.Create();
			SOCKADDR* socketaddress;
			struct sockaddr_in distant_addr;
			short port = (u_short)(4567);
			// remote address
			memset(&distant_addr, 0, sizeof(distant_addr));
			distant_addr.sin_family = AF_INET;
			distant_addr.sin_port = htons(port);
			inet_pton(AF_INET, protocol.c_str(), (&distant_addr.sin_addr));
			socketaddress = (SOCKADDR*)&(distant_addr);
			//

			// Truong hop k ket noi duoc
			if (!Client.Connect(socketaddress, sizeof(SOCKADDR))) {
				AfxMessageBox(_T("Failed to connect to server"));
				return nRetCode;
			}
			//

			SOCKET temp;
			temp = Client.Detach();
			Client.Attach(temp);
			cout << "Ket noi thanh cong\n";
			//
			int number = 1;
			do {
				char* name;
				char* phone_number;
				char* email;
				char* id;
				int valid;
				string name_to_find;

				int size;
				int a;
				int opt;
				vector<PersonalInfo> list;
				PersonalInfo person;
				Sleep(10);
				cout << "Nhap lua chon\n";
				cout << "1.Truy van thong tin 1 nguoi\n";
				cout << "2.Truy van thong tin moi nguoi\n";
				cout << "3.Tai anh cua 1 nguoi\n";
				cout << "4.Tai anh cua moi nguoi\n";
				cout << "0.Thoat\n";

				// gui lua chon
				cin >> opt ;
				cin.ignore();				
				Client.Send((int*)&opt, sizeof(opt), 0);
				//
				if (opt == 1)
				{
					// gui ten nguoi can tim
					cout << "Nhap ten nguoi can tim: ";
					getline(cin, name_to_find);
					a = name_to_find.length();
					Client.Send((int*)&a, sizeof(a), 0);
					Client.Send(name_to_find.c_str(), (int)strlen(name_to_find.c_str()), 0);

					
					Client.Receive((int*)&valid, sizeof(valid), 0);
					if (valid == -1)// khong tim thay nguoi
					{
						cout << "Khong co trong danh sach\n";
						system("pause");
						system("cls");
					}
					else// nhan thong tin nguoi can tim tu server
					{
						Client.Receive((int*)&a, sizeof(a), 0);
						name = new char[a + 1];
						Client.Receive(name, a, 0);
						name[a] = '\0';
						person.name = name;

						Client.Receive((int*)&a, sizeof(a), 0);
						phone_number = new char[a + 1];
						Client.Receive(phone_number, a, 0);
						phone_number[a] = '\0';
						person.phone_number = phone_number;

						Client.Receive((int*)&a, sizeof(a), 0);
						email = new char[a + 1];
						Client.Receive(email, a, 0);
						email[a] = '\0';
						person.email = email;

						Client.Receive((int*)&a, sizeof(a), 0);
						id = new char[a + 1];
						Client.Receive(id, a, 0);
						id[a] = '\0';
						person.id = id;

						list.push_back(person);
						showInfo(list);
						system("pause");
						system("cls");
					}
				}
				else if (opt == 2)
				{
					// lay do dai list
					Client.Receive((int*)&size, sizeof(size), 0);
					//
					for (int i = 0; i < size; i++)
					{
						Client.Receive((int*)&a, sizeof(a), 0);
						name = new char[a + 1];
						Client.Receive(name, a, 0);
						name[a] = '\0';
						person.name = name;

						Client.Receive((int*)&a, sizeof(a), 0);
						phone_number = new char[a + 1];
						Client.Receive(phone_number, a, 0);
						phone_number[a] = '\0';
						person.phone_number = phone_number;

						Client.Receive((int*)&a, sizeof(a), 0);
						email = new char[a + 1];
						Client.Receive(email, a, 0);
						email[a] = '\0';
						person.email = email;

						Client.Receive((int*)&a, sizeof(a), 0);
						id = new char[a + 1];
						Client.Receive(id, a, 0);
						id[a] = '\0';
						person.id = id;
						list.push_back(person);
					}
					showInfo(list);
					system("pause");
					system("cls");
				}
				else if (opt == 3)
				{
					// gui ten nguoi can tim
					cout << "Nhap ten nguoi can tim: ";
					getline(cin, name_to_find);

					a = name_to_find.length();
					Client.Send((int*)&a, sizeof(a), 0);
					Client.Send(name_to_find.c_str(), (int)strlen(name_to_find.c_str()), 0);

					// nhan phan hoi tu server
					Client.Receive((int*)&valid, sizeof(valid), 0);
					if (valid == -1)// khong tim thay nguoi hoac anh
					{
						cout << "khong tim thay nguoi hoac anh\n";
						system("pause");
						system("cls");
					}
					else
					{
						PPM image;
						char* temp;
						unsigned char c;
						//
						Client.Receive((int*)&a, sizeof(a), 0);
						temp = new char[a + 1];
						Client.Receive(temp, a, 0);
						temp[a] = '\0';
						image.setVersion(temp);
						//
						Client.Receive((int*)&a, sizeof(a), 0);
						image.setWidth(a);
						Client.Receive((int*)&a, sizeof(a), 0);
						image.setHeight(a);
						//tao mang con tro 2 chieu RGB
						image.create_image();
						//
						for (int i = 0; i < image.getHeight(); i++)
							for (int j = 0; j < image.getWidth(); j++)
							{
								
								Client.Receive(&c, sizeof(c), 0);
								image.image[i][j].r = c;

								Client.Receive(&c, sizeof(c), 0);
								image.image[i][j].g = c;

								Client.Receive(&c, sizeof(c), 0);
								image.image[i][j].b = c;
							}
						//
						image.save("Result.ppm");	
						cout << "Da nhan thanh cong anh\n";
					}
					system("pause");
					system("cls");
				}
				else if (opt == 4)
				{
					int number_of_pic;
					string result="Pic";
					//lay so luong anh
					Client.Receive((int*)&number_of_pic, sizeof(number_of_pic), 0);

					if (number_of_pic == 0)
					{
						cout << "Khong co anh\n";
					}
					else
					{
						for (int k = 0; k < number_of_pic; k++)
						{
							PPM image;
							char* temp;
							unsigned char c;
							//
							Client.Receive((int*)&a, sizeof(a), 0);
							temp = new char[a + 1];
							Client.Receive(temp, a, 0);
							temp[a] = '\0';
							image.setVersion(temp);
							//
							Client.Receive((int*)&a, sizeof(a), 0);
							image.setWidth(a);
							Client.Receive((int*)&a, sizeof(a), 0);
							image.setHeight(a);
							//tao mang con tro 2 chieu RGB
							image.create_image();
							//
							for (int i = 0; i < image.getHeight(); i++)
								for (int j = 0; j < image.getWidth(); j++)
								{

									Client.Receive(&c, sizeof(c), 0);
									image.image[i][j].r = c;

									Client.Receive(&c, sizeof(c), 0);
									image.image[i][j].g = c;

									Client.Receive(&c, sizeof(c), 0);
									image.image[i][j].b = c;
								}
							//
							result = result + to_string(k) + ".ppm";
							image.save(result);
							cout << "Da nhan thanh cong anh\n";
						}
					}
					system("pause");
					system("cls");
				}
				else/* if (opt == 0)*/
				{
					cout << "Exiting...\n";
					number = 0;
				}

			} while (number);
			Client.Close();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}

	return nRetCode;
}

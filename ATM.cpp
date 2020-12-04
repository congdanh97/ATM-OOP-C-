#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include <list>
#include <vector>
#include <locale.h>
using namespace std;
// Ham dung de comvert so tu dang number (1000000) sang dang co dau phay(1,000,000)
char* commaprint(unsigned int n)
{
	static int comma = '\0';
	static char retbuf[30];
	char* p = &retbuf[sizeof(retbuf) - 1];
	int i = 0;

	if (comma == '\0') {
		struct lconv* lcp = localeconv();
		if (lcp != NULL) {
			if (lcp->thousands_sep != NULL &&
				*lcp->thousands_sep != '\0')
				comma = *lcp->thousands_sep;
			else	comma = ',';
		}
	}

	*p = '\0';

	do {
		if (i % 3 == 0 && i != 0)
			* --p = comma;
		*--p = '0' + n % 10;
		n /= 10;
		i++;
	} while (n != 0);

	return p;
}

//Phí giao dịch
int nPhiGiaoDich(int nTienNhap, string sForm)
{
	if (sForm == "RUTTIEN")
	{
		if (nTienNhap <= 500000)
		{
			return 1100;
		}
		else if (nTienNhap <= 1000000)
		{
			return 3300;
		}
		else if (nTienNhap <= 10000000)
		{
			return 11000;
		}
		return 22000;
	}
	else if (sForm == "CHUYENTIEN")
	{
		if (nTienNhap <= 1000000)
		{
			return 11000;
		}
		else if (nTienNhap <= 5000000)
		{
			return 22000;
		}
		else if (nTienNhap <= 10000000)
		{
			return 33000;
		}
		return 44000;
	}
	return 0;
}

// Ham doi mau chu
void textcolor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}


class ATM {
public:
	friend class KhachHang;
	string MaCay ;
	int SoDuCayATM ;
	string DiaChi ;
public:
	//Đọc từ file input 1 Vector_List_ObjectKHách hàng
	void readDataATMFromFile(ifstream& Filein, ATM& a) 
	{
		getline(Filein, a.MaCay, ',');
		Filein >> a.SoDuCayATM;
		Filein.ignore(1, ',');
		getline(Filein, a.DiaChi);
	}

	//Thong tin cay ATM
	void thongTinCayATM(ATM a);
};
class KhachHang
{
private:
	string User;
	string Pass;
	string SoTaiKhoan;
	string HoTenKH;
	string LoaiTien;
	string ChiNhanh;
	unsigned int SoDuTK;
public:
	//Ghi dữ liệu
	void writeDataToFileKhachHang(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1, KhachHang& temp2);
	int maGiaoDich();
	// đăng nhập 
	string passwordInput(unsigned maxLength);
	// rút tiền
	void rutTien(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1, ATM a);
	void chuyenTien(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1,ATM a);
	void displayHistory(KhachHang& temp1);
	void read1LineInFileKhachHang(ifstream& Filein, KhachHang& temp);
	void readAllKhachHangInFileToVector(ifstream& Filein, vector <KhachHang>& Vector_List_ObjectKH);
	//void textcolor(WORD color);
	void login(vector <KhachHang>& Vector_List_ObjectKH, ATM a);
	// In du lieu Vector_List_ObjectKH  vao file sau khi thao tac xong
	void designViewDisplay(string sVien);
	//Thông tin tài khoản
	void thongTinTK(KhachHang& temp1);
	//Kiem tra time de chinh time va cau hinh cho cang hang thang cot khi hien thi lich su giao dich
	string kiemTraTime(string sX, KhachHang temp1);
	//Luu lich su giao dich vao file
	void writeHistoryDataToFile(KhachHang temp1, string type_perform, int nTienTru, KhachHang temp2, int PhiGD);
	//In Hoa don
	void printBill(KhachHang temp1, int nTienRut, int PhiGD, KhachHang temp2, string sForm, ATM a);
	// luu du lieu sau khi rut, chuyen tien vao file chua thong tin cay ATM
	void writeDataToFileATM(ATM a);
	// Doi mat khau
	void changePassword(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1, KhachHang& temp2);
	void recharge(ATM a);
};

//ma giao dich
int KhachHang::maGiaoDich()
{
	int nMaGiaoDich = 0;
	ifstream File1;
	File1.open("\IDgiaodich\\IDgiaodich.txt");
	File1 >> nMaGiaoDich;
	File1.close();
	ofstream File2;
	File2.open("\IDgiaodich\\IDgiaodich.txt");
	nMaGiaoDich = nMaGiaoDich + 1;
	File2 << nMaGiaoDich;
	File2.close();
	return nMaGiaoDich;
}



//Pass: Gioi han so ki tu nhap
string KhachHang:: passwordInput(unsigned maxLength)
{
	string pw = "";
	for (char c; (c = getch()); )
	{
		if (c == '\n' || c == '\r') { // \n là phím phím enter + \r là Nhảy về đầu hàng, không xuống hàng
			cout << "\n";
			break;
		}
		else if (c == '\b') { //phím backspace
			cout << "\b \b";
			if (!pw.empty()) pw.erase(pw.size() - 1);
		}
		else if (c == -32) { //phím mũi tên
			_getch(); //bỏ qua kí tự tiếp theo (hướng mũi tên)
		}
		else if (isprint(c) && pw.size() < maxLength) { //isprint tức là chỉ nhận những ký tự in ra được (có tính khoảng trắng)
			cout << '*';
			pw += c;
		}
	}
	return pw;
}

//Kiểm tra thời gian
string KhachHang:: kiemTraTime(string sX, KhachHang temp1)
{
	// làm dữ liệu lịch sử giao dịch \\thẳng hàng\\, thật đẹp để xuất ra màn hình
	time_t now = time('\0');
	tm* t = localtime(&now);
	if (sX == "ss")
	{
		if (t->tm_sec < 10)
		{
			return "0";
		}
	}
	if (sX == "m")
	{
		if ((t->tm_min) < 10)
		{
			return "0";
		}
	}
	if (sX == "hh")
	{
		if (t->tm_hour < 10)
		{
			return "0";
		}
	}
	if (sX == "dd")
	{
		if (t->tm_mday < 10)
		{
			return "0";
		}
	}
	if (sX == "mm")
	{
		if (t->tm_mon + 1 < 10)
		{
			return "0";
		}
	}
	if (sX == "100")
	{
		if (temp1.SoDuTK < 100000)
		{
			return " ";
		}
	}
	return "";
}


//Ghi lại lịch sử giao dịch
void KhachHang :: writeHistoryDataToFile( KhachHang temp1, string type_perform, int nTienTru,KhachHang temp2, int PhiGD)
{
	int nMaGiaoDich = 0;
	time_t now = time('\0');
	tm* t = localtime(&now);
	ofstream File;
	File.open("\LichSuGiaoDich\\" + temp1.SoTaiKhoan + ".txt", ios::app);
	if (type_perform == "CHUYENTIEN")
	{
		File << "\n" << kiemTraTime("hh", temp1) << t->tm_hour << ":" << kiemTraTime("m", temp1) << t->tm_min << ":" << kiemTraTime("ss", temp1) << t->tm_sec << "   " << kiemTraTime("dd", temp1) << t->tm_mday << "/" << kiemTraTime("mm", temp1) << 1 + t->tm_mon << "/" << 1900 + t->tm_year << " " << nTienTru << temp1.LoaiTien << kiemTraTime("100", temp1) << "  " << temp1.SoDuTK << temp1.LoaiTien << "   " << PhiGD << "     " << "chuyen" << "  " << temp2.SoTaiKhoan << "  " << temp2.HoTenKH << "   " << maGiaoDich();
	}
	else if (type_perform == "RUTTIEN")
	{
		File << "\n" << kiemTraTime("hh", temp1) << t->tm_hour << ":" << kiemTraTime("m", temp1) << t->tm_min << ":" << kiemTraTime("ss", temp1) << t->tm_sec << "   " << kiemTraTime("dd", temp1) << t->tm_mday << "/" << kiemTraTime("mm", temp1) << 1 + t->tm_mon << "/" << 1900 + t->tm_year << " " << nTienTru << temp1.LoaiTien << kiemTraTime("100", temp1) << "  " << temp1.SoDuTK << temp1.LoaiTien << "   " << PhiGD << "      " << "rut" << "     " << "Khong co" << "        " << "Khong co" << "       " << maGiaoDich();
	}
	else if (type_perform == "NHANTIEN")
	{
		File << "\n" << kiemTraTime("hh", temp1) << t->tm_hour << ":" << kiemTraTime("m", temp1) << t->tm_min << ":" << kiemTraTime("ss", temp1) << t->tm_sec << "   " << kiemTraTime("dd", temp1) << t->tm_mday << "/" << kiemTraTime("mm", temp1) << 1 + t->tm_mon << "/" << 1900 + t->tm_year << " " << nTienTru << temp2.LoaiTien << kiemTraTime("100", temp1) << "  " << temp1.SoDuTK << temp1.LoaiTien << "   " << "0" << "         " << "nhan" << "   " << temp2.SoTaiKhoan << "  " << temp2.HoTenKH << "   " << maGiaoDich();
	}
	else if (type_perform == "LOCKED")
	{
		File << "\n" << kiemTraTime("hh", temp1) << t->tm_hour << ":" << kiemTraTime("m", temp1) << t->tm_min << ":" << kiemTraTime("ss", temp1) << t->tm_sec << "   " << kiemTraTime("dd", temp1) << t->tm_mday << "/" << kiemTraTime("mm", temp1) << 1 + t->tm_mon << "/" << 1900 + t->tm_year << " " << "0" << temp1.LoaiTien << "       " << temp1.SoDuTK << temp1.LoaiTien << "   " << 0 << "         " << "khoa" << "    " << "Khong co" << "        " << "Khong co" << "       " << maGiaoDich();
	}
	else if (type_perform == "NAPTIEN")
	{
		File << "\n" << kiemTraTime("hh", temp1) << t->tm_hour << ":" << kiemTraTime("m", temp1) << t->tm_min << ":" << kiemTraTime("ss", temp1) << t->tm_sec << "   " << kiemTraTime("dd", temp1) << t->tm_mday << "/" << kiemTraTime("mm", temp1) << 1 + t->tm_mon << "/" << 1900 + t->tm_year << " " << nTienTru << temp1.LoaiTien << "  " << temp1.SoDuTK << temp1.LoaiTien << "   " << 0 << "         " << "add" << "     " << "Khong co" << "  " << "\t\tKhong co" << "" << maGiaoDich();
	}
	File.close();

}

//Đăng nhập
void KhachHang :: login(vector <KhachHang>& Vector_List_ObjectKH, ATM a)
{	KhachHang temp3;
	int nChon = 0;
	int i = 0;
	int nLan = 2;
	string sUsername = " ";
	string sPassword = " ";
	int flag = 0;
	int nDem = 0;
	designViewDisplay("LOGIN");
	cout << "\t\t\tLuu y: Nhap ";
	textcolor(4);
	cout << "ESC";
	textcolor(7);
	cout << " de thoat khoi chuong trinh\n" << endl;
	while (i < 3)
	{
		textcolor(2);
		cout << "\t\t\tNhap tai khoan: ";
		textcolor(7);
		fflush(stdin);
		getline(cin, sUsername);
		if (sUsername == "ESC" || sUsername == "esc")
		{
			textcolor(10);
			cout << "\n\t\t\tCam On Quy Khach Da Su Dung Dich Vu Hen Gap Lai " << endl;
			i = i + 4;
			textcolor(7);
		}
		else
		{
			textcolor(2);
			cout << "\t\t\tNhap pass word: ";
			textcolor(7);
			sPassword = passwordInput(6);
			vector<KhachHang>::iterator iter_name; // Vector_List_ObjectKHai báo con trỏ để duyệt	
			for (iter_name = Vector_List_ObjectKH.begin(); iter_name != Vector_List_ObjectKH.end(); iter_name++)
			{
				if (sUsername == iter_name->User && iter_name->Pass == "locked")
				{
					textcolor(4);
					cout << "\t\tTai khoan nay da bi khoa. Vui Long Lien He: 0942.942.243 !!!\a\n\n";
					textcolor(7);
					i += 4;
					break;
				}
				else if (sUsername == iter_name->User && sPassword == iter_name->Pass)
				{
					nDem += 5;
					KhachHang temp1 = *iter_name; //Code cu cua em
					
					//doan em sua lai
					//Khong phai truyen them doi tuong mơi temp1 vao ma dung luon thuoctinh doi tuong dang thao tac
					//User = iter_name->User;
					//Pass = iter_name->Pass;
					//SoTaiKhoan = iter_name->SoTaiKhoan;
					//HoTenKH = iter_name->HoTenKH;
					//LoaiTien = iter_name->LoaiTien;
					//ChiNhanh = iter_name->ChiNhanh;
					//SoDuTK = iter_name->SoDuTK;
					//
					i += 4;
					flag = 0;
					do {
						system("cls");
					cc:
						textcolor(10);
						cout << "\t\t\t\t Dang Nhap Thanh Cong - Ngan Hang OOP Bank \n" << endl;
						textcolor(7);
						designViewDisplay("MENU");
						textcolor(10);
						cout << "\t\t\tNhap so de thuc hien: ";
						textcolor(7);
						cin >> nChon;
					} while (nChon < 1 || nChon > 8);
					string sLuaChon = "";
					switch (nChon)
					{
					case 1:
						system("cls");
						//Rút tiền
						if (a.SoDuCayATM >= 50000)//check so du con lai cay ATM
						{
							rutTien(Vector_List_ObjectKH, temp1, a);
							textcolor(9);	
						}
						else
						{
							textcolor(10); cout << "\n\n\t\t\t\t -------- THONG BAO! -------- " << endl;
							textcolor(4); cout << "\t\t\t\t Cay ATM tam thoi het tien! "<<endl;
						}
						
						textcolor(9); cout << "\n\n\t\t\t Nhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de quay lai Menu. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
						fflush(stdin);
						cin >> sLuaChon;
						if (sLuaChon == "yes" || sLuaChon == "YES")
						{
							system("cls");
							goto cc;
						}
						else
						{
							textcolor(10);
							cout << "\n\t\t\tCam on quy khach da su dung dich vu, hen gap lai. \n";
							textcolor(7);
							break;
						}
					case 2:
						system("cls");
						//Chuyển Tiền
						chuyenTien(Vector_List_ObjectKH, temp1,a);
						textcolor(9);
						cout << "\n\n\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de quay lai Menu. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
						fflush(stdin); cin>> sLuaChon;
						if (sLuaChon == "yes" || sLuaChon == "YES")
						{
							system("cls");
							goto cc;
						}
						else
						{
							textcolor(10);
							cout << "\n\t\t\tCam on quy khach da su dung dich vu, hen gap lai. \n";
							textcolor(7);
							break;
						}
					case 3:
						system("cls");
						//Thông tin tài khoản
						thongTinTK(temp1);
						textcolor(9);
						cout << "\n\n\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de quay lai Menu. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
						fflush(stdin); cin >> sLuaChon;
						if (sLuaChon == "yes" || sLuaChon == "YES")
						{
							system("cls");
							goto cc;
						}
						else
						{
							textcolor(10);
							cout << "\n\t\t\tCam on quy khach da su dung dich vu, hen gap lai. \n";
							textcolor(7);
							break;
						}
					case 4:
						system("cls");
						//Lich su giao dich
						displayHistory(temp1);
						textcolor(9);
						cout << "\n\n\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de quay lai Menu. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
						fflush(stdin); cin>> sLuaChon;
						if (sLuaChon == "yes" || sLuaChon == "YES")
						{
							system("cls");
							goto cc;
						}
						else
						{
							textcolor(10);
							cout << "\n\t\t\tCam on quy khach da su dung dich vu, hen gap lai. \n";
							textcolor(7);
							break;
						}
					case 5:
						system("cls");
						//Doi Mat Khau
						changePassword(Vector_List_ObjectKH, temp1, temp3);
						break;
					case 6:
						//Do tien vao cay atm
						system("cls");
						recharge(a);
						textcolor(9);
						cout << "\n\n\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de quay lai Menu. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
						fflush(stdin); getline(cin, sLuaChon);
						if (sLuaChon == "yes" || sLuaChon == "YES")
						{
							system("cls");
							goto cc;
						}
						else
						{
							textcolor(10);
							cout << "\n\t\t\t Ban Da Nap Them Tien Thanh Cong Vao Cay ATM. \n";
							textcolor(7);
							break;
						}
					case 7:
						system("cls");
						//Thông tin tài khoản
						a.thongTinCayATM(a);

						textcolor(9);
						cout << "\n\n\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de quay lai Menu. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
						fflush(stdin); cin >> sLuaChon;
						if (sLuaChon == "yes" || sLuaChon == "YES")
						{
							system("cls");
							goto cc;
						}
						else
						{
							textcolor(10);
							cout << "\n\t\t\tCam on quy khach da su dung dich vu, hen gap lai. \n";
							textcolor(7);
							break;
						}
					case 8:
						//Thoat khoi giao dich
						textcolor(10);
						cout << "\n\t\t\tCam on quy khach da su dung dich vu, hen gap lai. \n";
						textcolor(7);
						break;
					}
				}
			}
			nDem++;
			if (nDem == 3)
			{
				system("cls");
				//designViewDisplay("LOGIN");
				textcolor(4);
				cout << "\t\t\tTai khoan ban da bi khoa !!! Vui Long Lien He: 0923.778.128 !!!\a\n\n";
				textcolor(7);
				i += 3;
				//vector<KhachHang>::iterator iter_name; // Vector_List_ObjectKHai báo con trỏ để duyệt
				for (iter_name = Vector_List_ObjectKH.begin(); iter_name != Vector_List_ObjectKH.end(); iter_name++)
				{
					if (sUsername == iter_name-> User)
					{
						KhachHang temp2;
						KhachHang temp1 = *iter_name;
						temp1.Pass = "locked";
						//Ghi dữ liệu lên file sau khi đổi MK
						writeDataToFileKhachHang(Vector_List_ObjectKH, temp1, temp2);
						writeHistoryDataToFile(temp1, "LOCKED", 0, temp2, 0);
						break;
					}
				}
			}
			else if (i < 3)
			{

				system("cls");
				//designViewDisplay("LOGIN");
				textcolor(4);
				cout << "\t\t\tTai khoan hoac mat khau khong dung.\a Con " << nLan-- << " lan dang nhap \n\n";
				textcolor(7);
				i++;
			}
		}
	}
}

//Đọc từ file input 1 Vector_List_ObjectKHách hàng
void KhachHang::read1LineInFileKhachHang(ifstream& Filein, KhachHang & temp) //Đọc từ file input 1 Vector_List_ObjectKHách hàng
{
	/*username, password,STK, Họ Tên, Số Dư, loại tiền, chi nhánh  */
	getline(Filein, temp.User, ',');
	getline(Filein, temp.Pass, ',');
	getline(Filein, temp.SoTaiKhoan, ',');
	getline(Filein, temp.HoTenKH, ',');
	Filein >> temp.SoDuTK;
	Filein.ignore(1, ',');
	getline(Filein, temp.LoaiTien, ',');
	getline(Filein, temp.ChiNhanh);
	Filein.ignore(0, '\n');
}
//Đọc từ file input toàn bộ Vector_List_ObjectKHách hàng
void KhachHang::readAllKhachHangInFileToVector(ifstream& Filein, vector <KhachHang>& Vector_List_ObjectKH)
{
	while (!Filein.eof() == true) // Nếu hàm Filein.eof() trả về true thì con trỏ chỉ thị con trỏ đang ở cuối file và ngược lại
	{
		//Bước 1: Đọc thông tin từng Vector_List_ObjectKHách hàng
		KhachHang temp;
		read1LineInFileKhachHang(Filein, temp); //bắt đầu đọc từ filein và thông tin Vector_List_ObjectKH;
		//Bước 2: Vector_List_ObjectKHởi tạo 1 cái Node Vector_List_ObjectKH;
		Vector_List_ObjectKH.push_back(temp);
		//Bước 3: Thêm 'Vector_List_ObjectKHách Hàng = Node p' Vào Danh Sách LK Đơn
	}
}

//Nap tien vao cay ATM
void KhachHang :: recharge(ATM a)
{
dd:
	system("cls");
	textcolor(6);
	cout << "\t\t\t\t*************************" << endl;
	cout << "\t\t\t\t*  ";
	textcolor(10);
	cout << "\tNAP TIEN\t"; textcolor(6);
	cout << "*" << endl;
	cout << "\t\t\t\t*************************" << endl;
	textcolor(7);
	int nNhapTienNap = 0;
	textcolor(4);
	cout << "\t\t\t Nhan Vien Hay Nhap Vao So Tien Can Do Vao Cay\n";
	cin >> nNhapTienNap;
	a.SoDuCayATM = a.SoDuCayATM +  nNhapTienNap;
	textcolor(10);
	cout << "\t\t\tBan da nap thanh cong " << nNhapTienNap << " " << "VND" << " vao cay ATM.\n";
	cout << "\t\t\tSo du hien tai: " << a.SoDuCayATM << " " << "VND" << "\n";
	textcolor(7);
	writeDataToFileATM(a);
}

//In hóa đơn
void KhachHang :: printBill( KhachHang temp1, int nTienRut, int PhiGD, KhachHang temp2, string sForm, ATM a)
{
	//string sMaATM = "999999"; //Mã trụ ATM
	time_t now = time('\0');
	tm* t = localtime(&now);
	if (sForm == "RUTTIEN")
	{
		textcolor(9);
		cout << "\t\t\t\t/-----------------------------------------------------\\\n";
		cout << "\t\t\t\t|                Ngan Hang OOP Bank                   |\n";
		cout << "\t\t\t\t|-----------------------------------------------------|\n";
		cout << "\t\t\t\t|\t\t\t\t\t\t      |\n";
		cout << "\t\t\t\t|\t\t    RUT TIEN MAT\t\t      |\n";
		cout << "\t\t\t\t|Ngay\t\t\tGio\t\t       May ATM|\n";
		cout << "\t\t\t\t|" << t->tm_mday << "/" << 1 + t->tm_mon << "/" << 1900 + t->tm_year << "\t\t" << t->tm_hour << ":" << t->tm_min << "\t\t\t" << a.MaCay << "|\n";
		cout << "\t\t\t\t|\t\t\t\t\t\t      |\n";
		cout << "\t\t\t\t|So tai khoan:  " << temp1.SoTaiKhoan << "\t\t\t      |\n";
		cout << "\t\t\t\t|So tien rut:   " << nTienRut << " " << temp1.LoaiTien << "\t\t\t      |\n";
		cout << "\t\t\t\t|So du tai khoan: " << commaprint( temp1.SoDuTK) << " " << temp1.LoaiTien << "\t\t\t      |\n";
		cout << "\t\t\t\t|So giao dich: " << maGiaoDich() << "\t\t\t\t      |\n";
		cout << "\t\t\t\t|\t\t\t\t\t\t      |\n";
		cout << "\t\t\t\t|Phi: " << PhiGD - (PhiGD / 11) << " VND\t\t\tVAT: " << PhiGD / 11 << " VND\t      |\n";
		cout << "\t\t\t\t|Phi in hoa don: 500 VND\t\t\t      |\n";
		cout << "\t\t\t\t|VAT in hoa don: 50 VND \t\t\t      |\n";
		cout << "\t\t\t\t|-----------------------------------------------------|\n";
		cout << "\t\t\t\t| \tDien thoai ho tro 24/24: 0941.942.243 \t      |\n";
		cout << "\t\t\t\t\\-----------------------------------------------------/\n";
		textcolor(7);
	}
	else if (sForm == "CHUYENTIEN")
	{
		textcolor(9);
		cout << "\t\t\t\t/-----------------------------------------------------\\\n";
		cout << "\t\t\t\t|                 Ngan Hang OOP Bank                  |\n";
		cout << "\t\t\t\t|-----------------------------------------------------|\n";
		cout << "\t\t\t\t|\t\t\t\t\t\t      |\n";
		cout << "\t\t\t\t|\t\t  CHUYEN TIEN TRONG NUOC\t      |\n";
		cout << "\t\t\t\t|Ngay\t\t\tGio\t\t       May ATM|\n";
		cout << "\t\t\t\t|" << t->tm_mday << "/" << 1 + t->tm_mon << "/" << 1900 + t->tm_year << "\t\t" << t->tm_hour << ":" << t->tm_min << "\t\t\t" << a.MaCay << "|\n";
		cout << "\t\t\t\t|\t\t\t\t\t\t      |\n";
		cout << "\t\t\t\t|So tai khoan:  " << temp1.SoTaiKhoan << "\t\t\t      |\n";
		cout << "\t\t\t\t|So tien chuyen:   " << nTienRut << " " << temp1.LoaiTien << "\t\t\t      |\n";
		cout << "\t\t\t\t|So du tai khoan: " << temp1.SoDuTK << " " << temp1.LoaiTien << "\t\t\t      |\n";
		cout << "\t\t\t\t|So giao dich: " << maGiaoDich() << "\t\t\t\t      |\n";
		cout << "\t\t\t\t|Chuyen den so tai khoan: " << temp2.SoTaiKhoan << "\t      |\n";
		cout << "\t\t\t\t|\t\t\t\t\t\t      |\n";
		cout << "\t\t\t\t|Phi: " << PhiGD - (PhiGD / 11) << " VND\t\t\tVAT: " << PhiGD / 11 << " VND\t      |\n";
		cout << "\t\t\t\t|Phi in hoa don: 500 VND\t\t\t      |\n";
		cout << "\t\t\t\t|VAT in hoa don: 50 VND \t\t\t      |\n";
		cout << "\t\t\t\t|-----------------------------------------------------|\n";
		cout << "\t\t\t\t| \tDien thoai ho tro 24/24: 0941.942.243 \t      |\n";
		cout << "\t\t\t\t\\-----------------------------------------------------/\n";
		textcolor(7);
	}


}
// 1.Rút tiền
void KhachHang:: rutTien(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1, ATM a)
{
	int nNhapTienRut = 0;
	int nN = 0;
	system("cls");
	do {
		designViewDisplay("RUTTIEN");
		designViewDisplay("MENURUTTIEN"); textcolor(10);
		cout << "\n\t\t\t\t\tNhap so can chon: "; textcolor(10);
		cin >> nN;
	} while (nN < 1 || nN > 6);
	switch (nN)
	{
	case 1:
		nNhapTienRut = 50000;
		break;
	case 2:
		nNhapTienRut = 100000;
		break;
	case 3:
		nNhapTienRut = 200000;
		break;
	case 4:
		nNhapTienRut = 500000;
		break;
	case 5:
		nNhapTienRut = 1000000;
		break;
	case 6:
		do {
		dd:
			system("cls");
			designViewDisplay("RUTTIEN");
			textcolor(4);
			cout << "\tSo tien nhap vao phai la boi cua 10.000vnd. Gioi han giao dich tu 50.000vnd den 50.000.000vnd\n";
			textcolor(10);
			cout << "\t\t\tNhap so tien can rut: ";
			textcolor(7);
			cin >> nNhapTienRut;
		} while (nNhapTienRut < 50000 || nNhapTienRut > 50000000);
		break;
	}
	if (nNhapTienRut % 10000 == 0)
	{
		double SoDuTKKhaDung = 0;
		SoDuTKKhaDung = temp1.SoDuTK - 50000;
		if (nNhapTienRut > SoDuTKKhaDung)
		{
			textcolor(4);
			cout << "\t\tSo du trong tai khoan khong du de thuc hien giao dich.\n";
			textcolor(7);
		}
		else
		{
			system("cls");
			designViewDisplay("RUTTIEN");
			string sKiemTra = "";
			textcolor(10);
			cout << "\t\t\t\tXac Nhan Rut Tien\n";
			textcolor(3);
			cout << "\t\t\tRut Tien Tu STK: " << temp1.SoTaiKhoan << endl;
			cout << "\t\t\tTen Chu The: " << temp1.HoTenKH << endl;
			cout << "\t\t\tSo Tien Rut: " << commaprint(nNhapTienRut) << " " << temp1.LoaiTien << endl;
			cout << "\t\t\tPhi Giao Dich: " << nPhiGiaoDich(nNhapTienRut, "RUTTIEN") - nPhiGiaoDich(nNhapTienRut, "RUTTIEN") / 11 << " " << temp1.LoaiTien << "   -   VAT: " << nPhiGiaoDich(nNhapTienRut, "RUTTIEN") / 11 << " " << temp1.LoaiTien << endl;
			cout << "\t\t\tSo Du Hien Tai: " << commaprint(temp1.SoDuTK) << " " << temp1.LoaiTien << endl;
			cout << "\t\t\tSo Du Con Lai: " << commaprint(temp1.SoDuTK - nNhapTienRut - nPhiGiaoDich(nNhapTienRut, "RUTTIEN")) << " " << temp1.LoaiTien << endl;
			cout << "\tNhap "; textcolor(10);
			cout << "Mat Khau"; textcolor(3);
			cout << " De Xac Nhan Lenh Rut Tien. Nhap "; textcolor(4);
			cout << "NO"; textcolor(3);
			cout << " De Huy Lenh Chuyen Tien: "; textcolor(7);
			sKiemTra = passwordInput(6);
			if (sKiemTra == temp1.Pass)
			{	
				//In hoa don rut tien?
				string ktra = "";
				KhachHang temp2;
				textcolor(9);
				cout << "\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de in hoa don. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" neu khong muon in hoa don: "; textcolor(7);
				fflush(stdin);
				getline(std::cin, ktra);
				cin >> ktra;
				if (ktra == "YES" || ktra == "yes")
				{
					int nPGD = nPhiGiaoDich(nNhapTienRut, "RUTTIEN");
					nPGD += 550;//tru them phi in hoa don

					//tru tien + thong bao rut tien thanh cong
					temp1.SoDuTK = temp1.SoDuTK - nNhapTienRut - nPGD;
					textcolor(10);
					cout << "\n\t\t\tBan da rut thanh cong: ";// << nNhapTienRut << " " << q->nData.LoaiTien << endl;
					cout << "\n\t\t\tSo du con lai: " << commaprint(temp1.SoDuTK) << " " << temp1.LoaiTien << endl;
					textcolor(7);
					//in hoa don
					printBill( temp1, nNhapTienRut, nPhiGiaoDich(nNhapTienRut, "RUTTIEN"), temp2, "RUTTIEN", a);
					//Ghi dữ liệu lên file sau khi rút tiền
					writeDataToFileKhachHang(Vector_List_ObjectKH, temp1, temp2);
					writeHistoryDataToFile( temp1, "RUTTIEN", nNhapTienRut, temp2, nPGD);
					//Tru tien con lai cua cay ATM
					a.SoDuCayATM = a.SoDuCayATM - nNhapTienRut - nPGD;
					writeDataToFileATM(a);

				}
				else
				{
					textcolor(10);
					cout << "\t\t\t\tBan da chon khong in hoa don !";
					textcolor(7);
					//tru tien + thong bao rut tien thanh cong
					temp1.SoDuTK = temp1.SoDuTK - nNhapTienRut - nPhiGiaoDich(nNhapTienRut, "RUTTIEN");
					textcolor(10);
					cout << "\n\t\t\tBan da rut thanh cong: ";// << nNhapTienRut << " " << q->nData.LoaiTien << endl;
					cout << "\n\t\t\tSo du con lai: " << commaprint(temp1.SoDuTK) << " " << temp1.LoaiTien << endl;
					//luu du lieu vao file
					writeDataToFileKhachHang(Vector_List_ObjectKH,temp1,temp2);
					writeHistoryDataToFile( temp1, "RUTTIEN", nNhapTienRut, temp2, nPhiGiaoDich(nNhapTienRut, "RUTTIEN"));
					//Tru tien con lai cua cay ATM
					a.SoDuCayATM = a.SoDuCayATM - nNhapTienRut - nPhiGiaoDich(nNhapTienRut, "RUTTIEN");
					writeDataToFileATM(a);
				}
			}
			else
			{
				textcolor(4);
				cout << "\t\t\tNhap sai ! Lenh Rut Tien Da Tam Dung\a\n";
				textcolor(7);
			}

		}
	}
	else
	{
		cout << "\a";
		goto dd;
	}
}

//2: Chuyển Tiền
void KhachHang :: chuyenTien(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1 , ATM a)
{
	int flag = 0;
	int i = 0;
	string sNhapSTKChuyen = " ";
	system("cls");
	designViewDisplay("CHUYENTIEN");
	cout << "\t\t\tLuu y: Nhap ";
	textcolor(4);
	cout << "ESC";
	textcolor(7);
	cout << " de thoat khoi chuong trinh\n" << endl;
	while (flag == 0)
	{
		textcolor(10);
		cout << "\t\t\tNhap so tai khoan can chuyen tien: "; textcolor(7);
		fflush(stdin);
		getline(cin, sNhapSTKChuyen);
		if (sNhapSTKChuyen == temp1.SoTaiKhoan)
		{
			textcolor(4);
			cout << "\t\t\tKhong the chuyen tien cho chinh minh !!!\n";
			textcolor(7);
		}
		else if (sNhapSTKChuyen == "ESC" || sNhapSTKChuyen == "esc")
		{
			flag = 5;
			textcolor(10);
			cout << "\t\t\tCam on quy khach da su dung dinh vu\n";
			textcolor(7);
		}
		else
		{	
			vector<KhachHang>::iterator iter_name; // Vector_List_ObjectKHai báo con trỏ để duyệt
			for (iter_name = Vector_List_ObjectKH.begin(); iter_name != Vector_List_ObjectKH.end(); iter_name++)
			{
				if (sNhapSTKChuyen == iter_name->SoTaiKhoan)
				{
					KhachHang temp2;
					temp2 = *iter_name;
					flag = 5;
					int nNhapTienChuyen = 0;
					do {
					gg:
						system("cls");
						designViewDisplay("CHUYENTIEN");
						textcolor(4);
						cout << "\tSo tien nhap vao phai la boi cua 50.000vnd. Gioi han giao dich tu 200.000vnd den 50.000.000vnd\n";
						textcolor(10);
						cout << "\t\t\tNhap so tien can chuyen: ";
						textcolor(7);
						cin >> nNhapTienChuyen;
						system("cls");
						if (nNhapTienChuyen % 50000 != 0)
						{
							goto gg;
						}

					} while (nNhapTienChuyen < 200000 || nNhapTienChuyen > 50000000);
					double SoDuTKKhaDung = 0.0;
					SoDuTKKhaDung = temp1.SoDuTK - 50000;
					if (nNhapTienChuyen >= 200000)
					{
						if (nNhapTienChuyen <= SoDuTKKhaDung)
						{
							system("cls");
							designViewDisplay("CHUYENTIEN");
							textcolor(10);
							cout << "\t\t\t\tXac Nhan Chuyen Tien\n";
							textcolor(3);
							cout << "\t\t\tChuyen Tien Den STK: " << temp2.SoTaiKhoan << endl;
							cout << "\t\t\tTen Nguoi Nhan: " << temp2.HoTenKH << endl;
							cout << "\t\t\tSo Tien Chuyen: " << nNhapTienChuyen << " " << temp2.LoaiTien << endl;
							//cout << "\t\t\tPhi Giao Dich: " << nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN") - nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN") / 11 << " " << q->nData.LoaiTien << "   -   VAT: " << nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN") / 11 << " " << q->nData.LoaiTien << endl;
							cout << "\t\t\tSo Du Hien Tai: " << temp1.SoDuTK << " " << temp2.LoaiTien << endl;
							//cout << "\t\t\tSo Du Con Lai: " << q->nData.SoDuTK - (float)nNhapTienChuyen - nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN") << " " << t->nData.LoaiTien << endl;
							string sLC = " ";
							cout << "\tNhap "; textcolor(10);
							cout << "Mat Khau"; textcolor(3);
							cout << " De Xac Nhan Lenh Chuyen Tien. Nhap "; textcolor(4);
							cout << "NO"; textcolor(3);
							cout << " De Huy Lenh Chuyen Tien: "; textcolor(7);
							sLC = passwordInput(6);
							if (sLC == temp1.Pass)
							{
								temp1.SoDuTK = temp1.SoDuTK - (float)nNhapTienChuyen- nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN");
								temp2.SoDuTK = temp2.SoDuTK + nNhapTienChuyen;
								textcolor(10);
								cout << "\t\t\tChuyen Tien Thanh Cong !!!\n";
								cout << "\t\t\tSo Du Con Lai: ";
								cout << commaprint(temp1.SoDuTK)  << " " << temp1.LoaiTien << endl; textcolor(7);
								
							

								//In hoa don:
								string ktra = "";
								textcolor(9);
								cout << "\t\tNhap \""; textcolor(10); cout << "YES"; textcolor(9); cout << "\" de in hoa don. Nhap \""; textcolor(4); cout << "NO"; textcolor(9); cout << "\" de thoat: "; textcolor(7);
								fflush(stdin);
								cin >> ktra;
								if (ktra == "YES" || ktra == "yes")
								{
									int nPGD = nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN");
									nPGD = nPGD + 550;
									printBill(temp1, nNhapTienChuyen, nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN"), temp1, "CHUYENTIEN", a);
									writeDataToFileKhachHang(Vector_List_ObjectKH, temp1, temp2);
									writeHistoryDataToFile(temp1, "CHUYENTIEN", nNhapTienChuyen, temp2, nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN"));
									writeHistoryDataToFile(temp2, "NHANTIEN", nNhapTienChuyen, temp1, 0);

									//Tru tien con lai cua cay ATM
									a.SoDuCayATM = a.SoDuCayATM - nNhapTienChuyen - nPGD;
									writeDataToFileATM(a);
								}
								else
								{
									textcolor(10);
									cout << "\t\t\t\tBan da chon khong in hoa don !";
									textcolor(7);
									int nPGD = nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN");
									writeDataToFileKhachHang(Vector_List_ObjectKH, temp1, temp2);
									writeHistoryDataToFile( temp1, "CHUYENTIEN", nNhapTienChuyen, temp2, nPhiGiaoDich(nNhapTienChuyen, "CHUYENTIEN"));
									writeHistoryDataToFile( temp2, "NHANTIEN", nNhapTienChuyen, temp1, 0);

									//Tru tien con lai cua cay ATM
									a.SoDuCayATM = a.SoDuCayATM - nNhapTienChuyen - nPGD;
									writeDataToFileATM(a);
								}
							}
							else
							{
								textcolor(4);
								cout << "\t\t\tNhap sai ! Lenh Chuyen Tien Da Tam Dung\a\n";
								textcolor(7);
							}
							break;
						}
						else if (nNhapTienChuyen > SoDuTKKhaDung)
						{
							system("cls");
							designViewDisplay("CHUYENTIEN");
							textcolor(4);
							cout << "\n\t\tSo du trong tai khoan khong du de thuc hien giao dich. Giao dich ket thuc !!!\n";
							textcolor(7);
							break;
						}
					}
				}
				else
				{
					flag = 0;
				}
			}
			if (flag == 0)
			{
				system("cls");
				designViewDisplay("CHUYENTIEN");
				cout << "\t\t\tLuu y: Nhap ";
				textcolor(4);
				cout << "ESE";
				textcolor(7);
				cout << " de thoat khoi chuong trinh\n" << endl;
				textcolor(4);
				cout << "\t\t\tQuy khach hay nhap chinh xac so tai khoan can chuyen den!\n";
				textcolor(7);
			}
		}
	}
}


//3. Thông tin tài khoản
void KhachHang :: thongTinTK(KhachHang& temp1)
{
	textcolor(6);
	cout << "\t\t\t\t*************************" << endl;
	cout << "\t\t\t\t*  ";
	textcolor(10);
	cout << "THONG TIN KHACH HANG "; textcolor(6);
	cout << "*" << endl;
	cout << "\t\t\t\t*************************" << endl;
	textcolor(7);
	textcolor(3);
	cout << "\n";
	cout << "\t\t\tHo va Ten: " << temp1.HoTenKH << endl;
	cout << "\t\t\tSo Tai Khoan: " << temp1.SoTaiKhoan << endl;
	cout << "\t\t\tSo Du Hien Tai: " << commaprint(temp1.SoDuTK)<< " " << temp1.LoaiTien << endl;
	cout << "\t\t\tChi Nhanh: " << temp1.ChiNhanh << endl;
	textcolor(7);
}

//4. Xuất lịch sử giao dịch ra màn hình
void KhachHang ::displayHistory(KhachHang& temp1)
{
	string a = "";
	ifstream File; //Mở File Input đưa dữ liệu vào
	File.open("\LichSuGiaoDich\\" + temp1.SoTaiKhoan + ".txt");
	cout << "		          =================>>>LICH SU GIAO DICH<<<=================\n\n";

	cout << "Gio GD     Ngay GD   Tien GD    So Du TK     Phi GD   Loai GD      SoTK Den        TenKH Den      Ma GD\n";
	while (!File.eof())
	{
		getline(File, a, '\n');
		cout << a << endl;
		Sleep(30);
	}
	File.close();
	cout << "\t\t___________________________________________________________________________\n";
}

//5.Đổi mật khẩu:
void KhachHang :: changePassword(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1, KhachHang &temp2)
{
	int nDem = 0;
	system("cls");
	designViewDisplay("DOIMK");
	string sPassHienTai = "";
dd:
	textcolor(10);
	cout << "\t\t\tNhap Mat Khau Hien Tai: ";
	textcolor(7);
	sPassHienTai = passwordInput(6);
	nDem++;
	if (sPassHienTai == temp1.Pass)
	{
		string sPassMoi = "";
		string sNhapLai = "";
		textcolor(4);
		cout << "\t\t\tLuu y: mat khau gom 6 chu so\n";
		textcolor(7);
		textcolor(10);
		cout << "\t\t\tNhap mat khau moi: ";
		textcolor(7);
		sPassMoi = passwordInput(6);
		textcolor(10);
		cout << "\t\t\tNhap lai mat khau moi: ";
		textcolor(7);
		sNhapLai = passwordInput(6);
		if (sPassMoi == sNhapLai)
		{
			if (sPassMoi.length() == 6)
			{
				temp1.Pass = sPassMoi;
				writeDataToFileKhachHang(Vector_List_ObjectKH, temp1, temp2);
				textcolor(10);
				cout << "\t\t\tBan da thay doi mat khau thanh cong !!!\n";
				cout << "\t\t\tVui long dang nhap lai !!!\n";
				textcolor(7);
			}
			else
			{
				system("cls");
				designViewDisplay("DOIMK");
				textcolor(4);
				cout << "\t\t\tLuu y: mat khau moi bao gom 6 chu so. Vui long nhap lai\n";
				textcolor(7);
				goto dd;
			}
		}
		else if (sPassMoi == "ESC" || sPassMoi == "esc" || sNhapLai == "ESC" || sNhapLai == "esc")
		{
			textcolor(10);
			cout << "\t\tBan da chon thoat khoi chuong trinh\n";
			textcolor(7);
		}
		else
		{
			system("cls");
			designViewDisplay("DOIMK");
			textcolor(4);
			cout << "\t\tMat khau khong trung khop vui long nhap lai: \n";
			textcolor(7);
			goto dd;
		}
	}
	else if (sPassHienTai == "esc" || sPassHienTai == "ESC")
	{
		textcolor(10);
		cout << "Ban da chon thoat khoi chuong trinh\n";
		textcolor(7);
	}
	else if (nDem == 3)
	{
		system("cls");
		designViewDisplay("DOIMK");
		textcolor(4);
		cout << "\t\tBan nhap sai qua 3 lan, tai khoan nay da bi khoa !!! Vui Long Lien He: 0923.778.128 !!!\a\n\n";
		textcolor(7);
		temp1.Pass = "locked";
		writeDataToFileKhachHang(Vector_List_ObjectKH, temp1, temp2);
		writeHistoryDataToFile(temp1, "LOCKED", 0, temp2, 0);
	}
	else
	{
		system("cls");
		designViewDisplay("DOIMK");
		textcolor(4);
		cout << "\t\tMat khau nhap vao sai, Vui long nhap lai.\n";
		cout << "\t\tNhap ESC de thoat khoi chuong trinh !\n";
		textcolor(7);
		goto dd;
	}
}
//Ghi lại dữ liệu lên file
void KhachHang :: writeDataToFileKhachHang(vector <KhachHang>& Vector_List_ObjectKH, KhachHang& temp1, KhachHang& temp2)
{
	ofstream File;
	File.open("input.txt");
	int Dem = 0;
	//for (Node* k = L.pHead; k != NULL; k = k->pNext)
	vector<KhachHang>::iterator iter_name; // Vector_List_ObjectKHai báo con trỏ để duyệt

	for (iter_name = Vector_List_ObjectKH.begin(); iter_name != Vector_List_ObjectKH.end(); iter_name++)
	{
		Dem++;
		if (iter_name->User == temp1.User )
		{
			*iter_name = temp1;
		}

		if (iter_name->User == temp2.User)
		{
			*iter_name = temp2;
		}

		File << iter_name->User << "," << iter_name->Pass << "," << iter_name->SoTaiKhoan << "," << iter_name->HoTenKH << "," << iter_name->SoDuTK << "," << iter_name->LoaiTien << "," << iter_name->ChiNhanh;
		if (Dem < Vector_List_ObjectKH.size()) //
		{
			File << endl;
		}
	}
	File.close();
}
//luu vao file thong tin ATM
void KhachHang::writeDataToFileATM(ATM a)
{
	ofstream File;
	File.open("input1.txt");
	File << a.MaCay << "," << a.SoDuCayATM << "," << a.DiaChi ;
	File.close();
}

//Viền thông báo
void KhachHang:: designViewDisplay(string sVien)
{
	if (sVien == "LOGIN")
	{
		textcolor(6);
		cout << "\t\t\t\t************************" << endl;
		cout << "\t\t\t\t*  ";
		textcolor(10);
		cout << "DANG NHAP HE THONG"; textcolor(6);
		cout << "  *" << endl;
		cout << "\t\t\t\t************************" << endl;
		textcolor(7);
	}
	else
	{
		if (sVien == "MENU")
		{
			textcolor(6);
			cout << "\t\t*************************************************************************" << endl;
			cout << "\t\t*  ";
			textcolor(10);
			cout << "\t\t    CHON PHUONG AN DE THUC HIEN"; textcolor(6);
			cout << "\t\t\t\t*" << endl;
			textcolor(6);
			cout << "\t\t*  ";
			textcolor(10);
			cout << "   1. Rut Tien\t\t\t2. Chuyen Tien"; textcolor(6);
			cout << "   \t\t*" << endl;
			textcolor(6);
			cout << "\t\t*  ";
			textcolor(10);
			cout << "   3. Thong Tin Tai Khoan\t\t4. Lich su giao dich"; textcolor(6);
			cout << "   \t\t*" << endl;
			textcolor(6);
			cout << "\t\t*  ";
			textcolor(10);
			cout << "   5. Doi Mat Khau \t\t\t6. Nap tien vao cay ATM\t"; textcolor(6);
			cout << "   \t*" << endl;
			cout << "\t\t*  ";
			textcolor(10);
			cout << "   7. Thong tin cay ATM \t\t8. Thoat"; 
			textcolor(6);cout << "    \t\t\t*" << endl;
			cout << "\t\t*************************************************************************" << endl;
			textcolor(7);
		}
		else if (sVien == "RUTTIEN")
		{
			textcolor(6);
			cout << "\t\t\t\t*************************" << endl;
			cout << "\t\t\t\t*  ";
			textcolor(10);
			cout << "\tRUT TIEN\t"; textcolor(6);
			cout << "*" << endl;
			cout << "\t\t\t\t*************************" << endl;
			textcolor(7);
		}
		else if (sVien == "DOIMK")
		{
			textcolor(6);
			cout << "\t\t\t\t*************************" << endl;
			cout << "\t\t\t\t*  ";
			textcolor(10);
			cout << "\tDOI MAT KHAU\t"; textcolor(6);
			cout << "*" << endl;
			cout << "\t\t\t\t*************************" << endl;
			textcolor(7);
		}
		else if (sVien == "CHUYENTIEN") {
			textcolor(6);
			cout << "\t\t\t\t*************************" << endl;
			cout << "\t\t\t\t*  ";
			textcolor(10);
			cout << "\tCHUYEN TIEN\t"; textcolor(6);
			cout << "*" << endl;
			cout << "\t\t\t\t*************************" << endl;
			textcolor(7);
		}
		else if (sVien == "MENURUTTIEN")
		{ //"; cout << "
			textcolor(10);
			cout << "\t\t\t\t\tVUI LONG CHON SO TIEN CAN RUT\n";
			textcolor(9);
			cout << "\t\t|\\----------------------------\\ \t\t /----------------------------/|" << endl;
			cout << "\t\t|"; textcolor(4); cout << "1"; textcolor(9); cout << "\\       50.000 VND           \\ \t\t/          100.000 VND       /"; textcolor(4); cout << "2"; textcolor(9); cout << "|" << endl; textcolor(10);
			cout << "\t\t| /                            / \t\t\\\                            \\ |" << endl;
			cout << "\t\t|/----------------------------/ \t\t \\----------------------------\\|" << endl;
			cout << endl; textcolor(9);
			cout << "\t\t|\\----------------------------\\ \t\t /----------------------------/|" << endl;
			cout << "\t\t|"; textcolor(4); cout << "3"; textcolor(9); cout << "\\      200.000 VND           \\ \t\t/          500.000 VND       /"; textcolor(4); cout << "4"; textcolor(9); cout << "|" << endl; textcolor(10);
			cout << "\t\t| /                            / \t\t\\\                            \\ |" << endl;
			cout << "\t\t|/----------------------------/ \t\t \\----------------------------\\|" << endl;
			cout << endl; textcolor(9);
			cout << "\t\t|\\----------------------------\\ \t\t /----------------------------/|" << endl;
			cout << "\t\t|"; textcolor(4); cout << "5"; textcolor(9); cout << "\\     1.000.000 VND          \\ \t\t/         Nhap So Khac       /"; textcolor(4); cout << "6"; textcolor(9); cout << "|" << endl; textcolor(10);
			cout << "\t\t| /                            / \t\t\\\                            \\ |" << endl;
			cout << "\t\t|/----------------------------/ \t\t \\----------------------------\\|" << endl; textcolor(7);
		}
		else if (sVien == "NAPTIEN")
		{
			textcolor(6);
			cout << "\t\t\t\t*************************" << endl;
			cout << "\t\t\t\t*  ";
			textcolor(10);
			cout << "\tNAP TIEN\t"; textcolor(6);
			cout << "*" << endl;
			cout << "\t\t\t\t*************************" << endl;
			textcolor(7);
		}
	}
}
//
void ATM::thongTinCayATM(ATM a)
{
	textcolor(6);
	cout << "\t\t\t\t*************************" << endl;
	cout << "\t\t\t\t*  ";
	textcolor(10);
	cout << "  THONG TIN CAY ATM  "; textcolor(6);
	cout << "*" << endl;
	cout << "\t\t\t\t*************************" << endl;
	textcolor(7);
	textcolor(3);
	cout << "\n";
	cout << "\t\t\t\t  Ngan Hang OOP Bank"<< endl;
	cout << "\t\t\t\tMa so cay ATM: " << a.MaCay << endl;
	cout << "\t\t\t\tDia chi cay ATM: " << a.DiaChi << endl;
	cout << "\t\t\t\tSo Du Hien Tai: " << commaprint(a.SoDuCayATM) << " " << "VND" << endl;
	textcolor(7);
}
int main()
{
	ATM a;// tao object a de luu du lieu ATM doc tu file
	ifstream File_ATM;//Mở file input đưa dữ liệu ATM vào
	File_ATM.open("input1.txt", ios::in);
	a.readDataATMFromFile(File_ATM, a);
	//cout << a.DiaChi;


	vector<KhachHang> Vector_List_ObjectKH;
	KhachHang kh_temp;
	
	try {
		ifstream File("input.txt");
		while (File)
		{
			//File.open("input.txt", ios::in);
			kh_temp.readAllKhachHangInFileToVector(File, Vector_List_ObjectKH);
			kh_temp.login(Vector_List_ObjectKH, a);
			system("pause");
		}
		File.close();

	}
	catch (std::exception e ) {
		cout << "khong the ket noi file " << endl;
	}
	//ifstream File; //Mở File Input đưa dữ liệu Khach Hang vào
	//File.open("input.txt", ios::in);
	//kh_temp.readAllKhachHangInFileToVector(File, Vector_List_ObjectKH); // hàm nhập dữ liệu từ file
	//File.close();
	// So phan tu khach hang trong list object 
	//std::cout << "size: " << Vector_List_ObjectKH.size() << "\n";
	
	
}
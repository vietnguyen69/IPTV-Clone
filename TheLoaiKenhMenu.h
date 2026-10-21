#ifndef THELOAIKENHMENU_H
#define THELOAIKENHMENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonUI.h"
#include "Repository.h"
#include "TheLoaiKenh.h"
#include "NoiDungVOD.h"

using namespace std;

class TheLoaiKenhMenu {
private:
    Repository<TheLoaiKenh>& repoTheLoai;
    Repository<NoiDungVOD>&   repoVOD;

    void themTheLoai() {
        printTitle("THEM THE LOAI KENH MOI");
        try {
            TheLoaiKenh tl;
            tl.inputData();

            if (repoTheLoai.add(tl)) {
                cout << "\n  >> Them the loai kenh thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma the loai '" << tl.getId() << "' da ton tai!" << endl;
            }
        } catch (const invalid_argument& e) {
            cout << "\n  [LOI] Du lieu khong hop le: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void hienThiTheLoai() {
        printTitle("DANH SACH THE LOAI KENH");
        displayTable(repoTheLoai.getAll());
    }

    void timKiemTheLoai() {
        printTitle("TIM KIEM THE LOAI KENH");
        string keyword;
        cout << "\n  Nhap tu khoa (ma hoac ten): ";
        getline(cin, keyword);

        if (keyword.empty()) {
            cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl;
            return;
        }

        vector<TheLoaiKenh> results = repoTheLoai.search(keyword);
        cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
        displayTable(results);
    }

    void capNhatTheLoai() {
        printTitle("CAP NHAT THE LOAI KENH");
        string id;
        cout << "\n  Nhap ma the loai can cap nhat: ";
        getline(cin, id);

        TheLoaiKenh* found = repoTheLoai.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay the loai voi ma '" << id << "'!" << endl;
            return;
        }

        cout << "\n  --- Thong tin hien tai ---" << endl;
        found->display();
        cout << "\n  --- Nhap thong tin moi ---" << endl;

        try {
            found->updateData();
            repoTheLoai.saveAfterUpdate();
            cout << "\n  >> Cap nhat thanh cong!" << endl;
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
            repoTheLoai.loadFromFile();
        }
    }

    void xoaTheLoai() {
        printTitle("XOA THE LOAI KENH");
        string id;
        cout << "\n  Nhap ma the loai can xoa: ";
        getline(cin, id);

        TheLoaiKenh* found = repoTheLoai.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay the loai voi ma '" << id << "'!" << endl;
            return;
        }

        cout << "\n  --- Thong tin se bi xoa ---" << endl;
        found->display();

        bool dangSuDung = false;
        for (const auto& vod : repoVOD.getAll()) {
            if (vod.getMaTheLoai() == id) {
                dangSuDung = true;
                break;
            }
        }

        if (dangSuDung) {
            cout << "\n  [LOI] Khong the xoa! Co noi dung VOD dang su dung the loai nay." << endl;
            return;
        }

        string confirm;
        cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
        getline(cin, confirm);

        if (confirm == "Y" || confirm == "y") {
            if (repoTheLoai.remove(id)) cout << "\n  >> Xoa thanh cong!" << endl;
            else cout << "\n  [LOI] Xoa that bai!" << endl;
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

public:
    TheLoaiKenhMenu(Repository<TheLoaiKenh>& rTL, Repository<NoiDungVOD>& rVOD)
        : repoTheLoai(rTL), repoVOD(rVOD) {}

    void chay() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY THE LOAI KENH");
            cout << "\n  1. Them the loai kenh\n  2. Hien thi danh sach\n  3. Tim kiem\n  4. Cap nhat thong tin\n  5. Xoa the loai\n  0. << Quay lai menu chinh" << endl;
            printLine();
            cout << "  Lua chon cua ban: ";
            string input;
            getline(cin, input);
            try { choice = stoi(input); } catch (...) { choice = -1; }

            switch (choice) {
                case 1: themTheLoai(); pauseScreen(); break;
                case 2: hienThiTheLoai(); pauseScreen(); break;
                case 3: timKiemTheLoai(); pauseScreen(); break;
                case 4: capNhatTheLoai(); pauseScreen(); break;
                case 5: xoaTheLoai(); pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le!" << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif // THELOAIKENHMENU_H

#ifndef NOIDUNGVODMENU_H
#define NOIDUNGVODMENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonUI.h"
#include "Repository.h"
#include "NoiDungVOD.h"
#include "TheLoaiKenh.h"

using namespace std;

class NoiDungVODMenu {
private:
    Repository<NoiDungVOD>&   repoVOD;
    Repository<TheLoaiKenh>& repoTheLoai;

    void themVOD() {
        printTitle("THEM NOI DUNG VOD MOI");
        if (repoTheLoai.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co the loai kenh nao!" << endl;
            return;
        }

        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());
        cout << endl;
        try {
            NoiDungVOD vod;
            vod.inputData();
            if (repoTheLoai.findById(vod.getMaTheLoai()) == nullptr) {
                throw invalid_argument("Ma the loai '" + vod.getMaTheLoai() + "' khong ton tai!");
            }
            if (repoVOD.add(vod)) {
                cout << "\n  >> Them noi dung VOD thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma VOD '" << vod.getId() << "' da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void hienThiVOD() {
        printTitle("DANH SACH NOI DUNG VOD");
        displayTable(repoVOD.getAll());
    }

    void timKiemVOD() {
        printTitle("TIM KIEM NOI DUNG VOD");
        string keyword;
        cout << "\n  Nhap tu khoa (ma hoac tieu de): ";
        getline(cin, keyword);
        if (keyword.empty()) return;
        vector<NoiDungVOD> results = repoVOD.search(keyword);
        cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
        displayTable(results);
    }

    void capNhatVOD() {
        printTitle("CAP NHAT NOI DUNG VOD");
        string id;
        cout << "\n  Nhap ma VOD can cap nhat: ";
        getline(cin, id);
        NoiDungVOD* found = repoVOD.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay ma '" << id << "'!" << endl;
            return;
        }

        found->display();
        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());

        NoiDungVOD backup = *found;
        try {
            found->updateData();
            if (repoTheLoai.findById(found->getMaTheLoai()) == nullptr) {
                *found = backup;
                throw invalid_argument("Ma the loai khong ton tai!");
            }
            repoVOD.saveAfterUpdate();
            cout << "\n  >> Cap nhat thanh cong!" << endl;
        } catch (const exception& e) {
            *found = backup;
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void xoaVOD() {
        printTitle("XOA NOI DUNG VOD");
        string id;
        cout << "\n  Nhap ma VOD can xoa: ";
        getline(cin, id);
        NoiDungVOD* found = repoVOD.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay ma '" << id << "'!" << endl;
            return;
        }
        found->display();

        string confirm;
        cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
        getline(cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            if (repoVOD.remove(id)) cout << "\n  >> Xoa thanh cong!" << endl;
            else cout << "\n  [LOI] Xoa that bai!" << endl;
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

public:
    NoiDungVODMenu(Repository<NoiDungVOD>& rVOD, Repository<TheLoaiKenh>& rTL)
        : repoVOD(rVOD), repoTheLoai(rTL) {}

    void chay() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY NOI DUNG VOD");
            cout << "\n  1. Them VOD\n  2. Hien thi danh sach\n  3. Tim kiem\n  4. Cap nhat\n  5. Xoa\n  0. << Quay lai" << endl;
            printLine();
            cout << "  Lua chon cua ban: ";
            string input;
            getline(cin, input);
            try { choice = stoi(input); } catch (...) { choice = -1; }

            switch (choice) {
                case 1: themVOD(); pauseScreen(); break;
                case 2: hienThiVOD(); pauseScreen(); break;
                case 3: timKiemVOD(); pauseScreen(); break;
                case 4: capNhatVOD(); pauseScreen(); break;
                case 5: xoaVOD(); pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le!" << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif // NOIDUNGVODMENU_H

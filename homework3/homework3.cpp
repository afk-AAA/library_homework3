#include <iostream>
#include <list>     //這是一種雙向鏈表，可以在任意位置快速插入和刪除元素
#include <forward_list> //這是一種單向鏈表，佔用的記憶體更少，但只能從前面插入或刪除元素
#include <vector>   //這是一種動態數組，支援隨機存取，適合需要快速存取元素且知道元素數量會變化的情況
#include <string>
#include <algorithm>//提供了許多常用的演算法，例如排序、查找等
#include <regex>    //提供了正則表達式的支持，允許用戶對字串進行複雜的匹配和驗證，這裡用於驗證圖書編號的格式

using namespace std;

// 定義書籍結構
struct Book {
    string title;  // 書名
    string author; // 作者
    string id;     // 圖書編號
};

// 定義借閱者結構
struct Borrower {
    string name;                // 借閱者姓名
    vector<string> borrowedBooks; // 借閱的圖書編號列表
};

// 圖書館類別
class Library {
private:
    list<Book> books;  // 存放書籍的列表
    forward_list<Borrower> borrowers; // 存放借閱者的列表

    // 驗證圖書編號格式是否合法
    bool isValidBookId(const string& id) {
        return regex_match(id, regex("^[A-Za-z]\\d{4}$")); // 驗證格式為字母加4個數字
    }

public:
    // 建構子，初始化測試資料
    Library() {
        // 加入測試圖書資料
        books = {
            {"紅樓夢", "曹雪芹", "A1234"},
            {"西遊記", "吳承恩", "B2345"},
            {"水滸傳", "施耐庵", "B3456"},
            {"三國演義", "羅貫中", "C4567"},
            {"金瓶梅", "蘭陵笑笑生", "C5678"},
            {"聊齋志異", "蒲松齡", "D6789"},
            {"儒林外史", "吳敬梓", "D7890"},
            {"封神演義", "許仲琳", "E8901"},
            {"鏡花緣", "李汝珍", "E9012"},
            {"老殘遊記", "劉鶚", "F0123"}
        };

        // 加入測試借閱者資料
        borrowers.push_front({ "小明", {"A1234", "B2345"} });
        borrowers.push_front({ "小華", {"C4567"} });
        borrowers.push_front({ "小美", {"D6789", "E8901"} });
        borrowers.push_front({ "小強", {"F0123"} });
        borrowers.push_front({ "小麗", {"B3456", "C5678"} });
    }

    // 添加新書籍
    void addBook() {
        Book newBook; // 新建一本書
        cout << "請輸入書名: ";
        cin >> ws; // 丟棄前導空白
        getline(cin, newBook.title); // 讀取書名
        cout << "請輸入作者: ";
        cin >> ws;
        getline(cin, newBook.author); // 讀取作者

        string id;
        do {
            cout << "請輸入圖書編號 ( 字母加 4 數字): ";
            cin >> id;
            // 驗證圖書編號格式
            if (!isValidBookId(id)) {
                cout << "圖書編號格式不正確。請再試一次。" << endl;
            }
        } while (!isValidBookId(id));
        newBook.id = id; // 設定圖書編號

        books.push_back(newBook); // 將新書添加到列表
        cout << "圖書添加成功！" << endl;
    }

    // 刪除書籍
    void removeBook() {
        string id;
        cout << "請輸入要刪除的圖書編號: ";
        cin >> id;
        // 在書籍列表中查找圖書編號
        auto it = find_if(books.begin(), books.end(), [&id](const Book& book) {
            return book.id == id; // 返回符合條件的書籍
            });
        if (it != books.end()) {
            books.erase(it); // 刪除找到的書籍
            cout << "圖書刪除成功！" << endl;
        }
        else {
            cout << "未找到該圖書。" << endl; // 未找到書籍
        }
    }

    // 搜索書籍
    void searchBook() {
        string id;
        cout << "請輸入要搜索的圖書編號: ";
        cin >> id;
        // 查找書籍
        auto it = find_if(books.begin(), books.end(), [&id](const Book& book) {
            return book.id == id; // 返回符合條件的書籍
            });
        if (it != books.end()) {
            cout << "找到圖書: " << it->title << "，作者: " << it->author << endl; // 顯示書籍信息
        }
        else {
            cout << "未找到該圖書。" << endl; // 未找到書籍
        }
    }

    // 列出所有書籍
    void listBooks() {
        // 根據圖書編號排序
        books.sort([](const Book& a, const Book& b) {
            return a.id < b.id; // 根據編號排序
            });
        cout << "圖書館的圖書列表:" << endl;
        // 遍歷書籍列表並顯示
        for (const auto& book : books) {
            cout << "書名: " << book.title << " 作者: " << book.author << " 圖書編號: " << book.id << endl;
        }
    }

    // 添加借閱者
    void addBorrower() {
        Borrower newBorrower; // 新建一個借閱者
        cout << "請輸入借閱者姓名: ";
        cin >> ws;
        getline(cin, newBorrower.name); // 讀取借閱者姓名
        borrowers.push_front(newBorrower); // 將新借閱者添加到列表
        cout << "借閱者添加成功！" << endl;
    }

    // 刪除借閱者
    void removeBorrower() {
        string name;
        cout << "請輸入要刪除的借閱者姓名: ";
        cin >> ws;
        getline(cin, name);

        auto prev = borrowers.before_begin(); // 獲取前一個元素的迭代器
        for (auto it = borrowers.begin(); it != borrowers.end(); ++it) {
            if (it->name == name) { // 查找借閱者姓名
                borrowers.erase_after(prev); // 刪除找到的借閱者
                cout << "借閱者刪除成功！" << endl;
                return;
            }
            prev++; // 更新前一個元素的迭代器
        }
        cout << "未找到該借閱者。" << endl; // 未找到借閱者
    }

    // 搜索借閱者
    void searchBorrower() {
        string name;
        cout << "請輸入要搜索的借閱者姓名: ";
        cin >> ws;
        getline(cin, name);

        // 查找借閱者
        for (const auto& borrower : borrowers) {
            if (borrower.name == name) {
                cout << "找到借閱者: " << borrower.name << endl; // 顯示借閱者姓名
                return;
            }
        }
        cout << "未找到該借閱者。" << endl; // 未找到借閱者
    }

    // 列出所有借閱者
    void listBorrowers() {
        cout << "借閱者列表:" << endl;
        // 遍歷借閱者列表並顯示
        for (const auto& borrower : borrowers) {
            cout << "姓名: " << borrower.name;
            for (const auto& bookId : borrower.borrowedBooks) {
                cout << " 圖書編號: " << bookId; // 顯示借閱者所借的圖書編號
            }
            cout << endl; // 換行
        }
    }
};

// 主函數
int main() {
    Library library; // 創建圖書館實例

    // 命令循環
    int choice;
    while (true) {
        cout << "\n圖書館管理系統\n";
        cout << "1. 添加圖書\n";
        cout << "2. 刪除圖書\n";
        cout << "3. 搜索圖書\n";
        cout << "4. 列出圖書\n";
        cout << "5. 添加借閱者\n";
        cout << "6. 刪除借閱者\n";
        cout << "7. 搜索借閱者\n";
        cout << "8. 列出借閱者\n";
        cout << "9. 退出\n";
        cout << "請輸入您的選擇: ";
        cin >> choice; // 讀取用戶的選擇

        // 根據用戶的選擇執行對應的功能
        switch (choice) {
        case 1: library.addBook(); break;
        case 2: library.removeBook(); break;
        case 3: library.searchBook(); break;
        case 4: library.listBooks(); break;
        case 5: library.addBorrower(); break;
        case 6: library.removeBorrower(); break;
        case 7: library.searchBorrower(); break;
        case 8: library.listBorrowers(); break;
        case 9: return 0; // 退出程式
        default: cout << "無效選擇。請再試一次。" << endl; // 處理無效選擇
        }
    }

    return 0; // 返回0表示程式正常結束
}

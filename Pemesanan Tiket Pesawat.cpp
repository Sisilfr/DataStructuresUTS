#include <iostream>
#include <string>
#include <queue>
#include <fstream> // Untuk menyimpan file
using namespace std;

// Tree node for Ticket Category
struct TicketCategory {
    string category;
    TicketCategory* left;
    TicketCategory* right;

    TicketCategory(string c) : category(c), left(NULL), right(NULL) {}
};

// Linked List node for Customer Order
struct Order {
    string name;
    string destination;
    string category;
    int quantity;
    Order* next;

    Order(string n, string d, string c, int q) : name(n), destination(d), category(c), quantity(q), next(NULL) {}
};

class OrderList {
public:
    Order* head;

    OrderList() : head(NULL) {}

    Order* addOrder(string name, string destination, string category, int quantity) {
        Order* newOrder = new Order(name, destination, category, quantity);
        if (!head) head = newOrder;
        else {
            Order* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newOrder;
        }
        return newOrder;
    }

    void displayOrders() {
        if (!head) {
            cout << "Belum ada pesanan.\n";
            return;
        }
        Order* temp = head;
        int i = 1;
        while (temp) {
            cout << i++ << ". " << temp->name << " - Tujuan: " << temp->destination
                 << ", Kategori: " << temp->category << ", Jumlah Tiket: " << temp->quantity << endl;
            temp = temp->next;
        }
    }

    void deleteOrder(int index) {
        if (!head || index < 1) {
            cout << "Nomor pesanan tidak valid.\n";
            return;
        }

        Order* temp = head;
        if (index == 1) {
            head = head->next;
            delete temp;
            cout << "Pesanan berhasil dihapus.\n";
            return;
        }

        Order* prev = nullptr;
        int count = 1;
        while (temp && count < index) {
            prev = temp;
            temp = temp->next;
            count++;
        }

        if (!temp) {
            cout << "Nomor pesanan tidak ditemukan.\n";
            return;
        }

        prev->next = temp->next;
        delete temp;
        cout << "Pesanan berhasil dihapus.\n";
    }

    void editOrder(int index) {
        if (!head || index < 1) {
            cout << "Nomor pesanan tidak valid.\n";
            return;
        }

        Order* temp = head;
        int count = 1;
        while (temp && count < index) {
            temp = temp->next;
            count++;
        }

        if (!temp) {
            cout << "Nomor pesanan tidak ditemukan.\n";
            return;
        }

        // Menanyakan data baru untuk pesanan
        cout << "Edit pesanan ke-" << index << endl;
        cout << "Masukkan nama baru: ";
        getline(cin, temp->name);
        cout << "Masukkan tujuan baru: ";
        getline(cin, temp->destination);
        cout << "Masukkan kategori tiket baru (Ekonomi/Bisnis/VIP): ";
        getline(cin, temp->category);
        cout << "Masukkan jumlah tiket baru: ";
        cin >> temp->quantity;
        cin.ignore();

        cout << "Pesanan berhasil diperbarui.\n";
    }

    void saveToFile() {
        ofstream outFile("orders.txt");
        if (!outFile) {
            cout << "Gagal membuka file untuk menyimpan pesanan.\n";
            return;
        }

        Order* temp = head;
        while (temp) {
            outFile << temp->name << "\n" << temp->destination << "\n"
                    << temp->category << "\n" << temp->quantity << "\n";
            temp = temp->next;
        }

        outFile.close();
        cout << "Pesanan berhasil disimpan ke file.\n";
    }
};

// Queue for order processing
queue<Order*> orderQueue;

// Function to process orders
void processOrders() {
    if (orderQueue.empty()) {
        cout << "Antrian kosong. Tidak ada pesanan yang diproses.\n";
        return;
    }
    cout << "\n=== Memproses Pesanan ===\n";
    int i = 1;
    while (!orderQueue.empty()) {
        Order* current = orderQueue.front();
        orderQueue.pop();
        cout << i++ << ". Pesanan untuk " << current->name << " (" 
             << current->category << ") ke " << current->destination 
             << " sebanyak " << current->quantity << " tiket - DIPROSES\n";
    }
}

// Tree: build and display categories
TicketCategory* buildCategoryTree() {
    TicketCategory* root = new TicketCategory("Tiket");
    root->left = new TicketCategory("Ekonomi");
    root->right = new TicketCategory("Bisnis");
    root->right->right = new TicketCategory("VIP");
    return root;
}

void displayCategories(TicketCategory* root) {
    if (root) {
        displayCategories(root->left);
        if (root->category != "Tiket")
            cout << "- " << root->category << endl;
        displayCategories(root->right);
    }
}

// === Main Menu ===
int main() {
    OrderList orderList;
    TicketCategory* categoryRoot = buildCategoryTree();

    int choice;
    do {
        cout << "\n=== Aplikasi Pemesanan Tiket Pesawat ===\n";
        cout << "1. Lihat Kategori Tiket\n";
        cout << "2. Buat Pemesanan Tiket\n";
        cout << "3. Lihat Daftar Pemesan\n";
        cout << "4. Hapus Pesanan\n";
        cout << "5. Edit Pesanan\n";
        cout << "6. Simpan ke File\n";
        cout << "7. Proses Pemesanan Tiket\n";
        cout << "8. Keluar\n";
        cout << "Pilih menu: ";
        cin >> choice;
        cin.ignore(); // ignore newline

        if (choice == 1) {
            cout << "\nKategori Tiket yang Tersedia:\n";
            displayCategories(categoryRoot);
        } else if (choice == 2) {
            string name, destination, category;
            int quantity;

            cout << "Masukkan nama: ";
            getline(cin, name);
            cout << "Masukkan tujuan: ";
            getline(cin, destination);
            cout << "Masukkan kategori tiket (Ekonomi/Bisnis/VIP): ";
            getline(cin, category);
            cout << "Masukkan jumlah tiket: ";
            cin >> quantity;
            cin.ignore();

            Order* newOrder = orderList.addOrder(name, destination, category, quantity);
            orderQueue.push(newOrder);

            cout << "Pesanan berhasil ditambahkan!\n";
        } else if (choice == 3) {
            cout << "\n=== Daftar Pemesan ===\n";
            orderList.displayOrders();
        } else if (choice == 4) {
            cout << "\n=== Hapus Pesanan ===\n";
            orderList.displayOrders();
            int index;
            cout << "Masukkan nomor pesanan yang ingin dihapus: ";
            cin >> index;
            cin.ignore();
            orderList.deleteOrder(index);
        } else if (choice == 5) {
            cout << "\n=== Edit Pesanan ===\n";
            orderList.displayOrders();
            int index;
            cout << "Masukkan nomor pesanan yang ingin diedit: ";
            cin >> index;
            cin.ignore();
            orderList.editOrder(index);
        } else if (choice == 6) {
            orderList.saveToFile();
        } else if (choice == 7) {
            processOrders();
        } else if (choice == 8) {
            cout << "Terima kasih telah menggunakan aplikasi.\n";
        } else {
            cout << "Pilihan tidak valid.\n";
        }

    } while (choice != 8);

    return 0;
}



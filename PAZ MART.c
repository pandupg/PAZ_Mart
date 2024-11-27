#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define MAX_PRODUCTS 10
#define MAX_CUSTOMERS 100
#define MAX_CART 10
#define EMPLOYEE_CODE "2711"  // Kode untuk akses mode karyawan

typedef struct {
    char name[30];
    float price;
    int stock;
} Product;

typedef struct {
    Product product;
    int quantity;
} CartItem;

typedef struct {
    char nrp[15];  // NRP (harus unik)
    char name[50]; // Nama pelanggan
} Customer;

Product products[MAX_PRODUCTS] = {
    {"Roti", 5000.0, 20},
    {"Minuman", 3000.0, 30},
    {"Cokelat", 10000.0, 15},
    {"Susu", 8000.0, 25},
    {"Kopi", 7000.0, 40}
};
int productCount = 5;  // Jumlah produk awal

Customer customers[MAX_CUSTOMERS];
int customerCount = 0; // Jumlah pelanggan terdaftar
CartItem cart[MAX_CART];
int cartItemCount = 0; // Jumlah barang dalam keranjang
char currentCustomerName[50] = ""; // Nama pelanggan yang sedang login

// Fungsi untuk memvalidasi apakah NRP hanya berisi angka
int isNumeric(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// Fungsi menampilkan daftar produk dalam tabel
void displayProducts() {
    printf("\n+-----------------------------------+-------------------+-------------+\n");
    printf("| No | Nama Produk                  | Harga             | Stok        |\n");
    printf("+-----------------------------------+-------------------+-------------+\n");
    for (int i = 0; i < productCount; i++) {
        printf("| %-2d | %-28s | Rp %-14.2f | %-11d |\n", i + 1, products[i].name, products[i].price, products[i].stock);
    }
    printf("+-----------------------------------+-------------------+-------------+\n");
}

// Fungsi untuk menambahkan barang ke keranjang
void addToCart() {
    int productIndex, quantity;
    char choice = 'Y';

    while(choice == 'Y' || choice == 'y'){
        displayProducts();
        printf("Masukkan nomor produk yang ingin dibeli: ");
        scanf("%d", &productIndex);

        if (productIndex < 1 || productIndex > productCount) {
            printf("Nomor produk tidak valid.\n");
            return;
        }

        printf("Masukkan jumlah yang ingin dibeli: ");
        scanf("%d", &quantity);

        if (products[productIndex - 1].stock < quantity) {
            printf("Stok tidak mencukupi.\n");
            return;
        }

        // Kurangi stok produk dan tambahkan ke keranjang
        products[productIndex - 1].stock -= quantity;
        cart[cartItemCount].product = products[productIndex - 1];
        cart[cartItemCount].quantity = quantity;
        cartItemCount++;

        printf("Produk %s berhasil ditambahkan ke keranjang.\n", products[productIndex - 1].name);

        ulangi:

        printf("Apakah mau menambah keranjang lagi (Y/T)? ");
        scanf(" %c", &choice);

        if(choice != 'T' && choice != 't' && choice != 'Y' && choice != 'y'){
            printf("Silahkan masukkan pilihan yang benar | %c bukan pilihan yang benar\n", choice);
            goto ulangi;
        }
    }
}

// Fungsi menampilkan keranjang belanja
void displayCart() {
    printf("\n+-----------------------------------+-------------------+---------------+\n");
    printf("| No | Nama Produk                  | Harga             | Jumlah        |\n");
    printf("+-----------------------------------+-------------------+---------------+\n");
    float total = 0;
    for (int i = 0; i < cartItemCount; i++) {
        printf("| %-2d | %-28s | Rp %-14.2f | %-13d |\n",
               i + 1,
               cart[i].product.name,
               cart[i].product.price,
               cart[i].quantity);
        total += cart[i].product.price * cart[i].quantity;
    }
    printf("+-----------------------------------+-------------------+---------------+\n");
    printf("Total Belanja: Rp %.2f\n", total);
}

// Fungsi untuk menambah produk baru
void addNewProduct() {
    if (productCount >= MAX_PRODUCTS) {
        printf("Tidak dapat menambahkan produk baru. Kapasitas maksimal tercapai.\n");
        return;
    }

    printf("Masukkan nama produk baru: ");
    scanf(" %[^\n]", products[productCount].name);
    printf("Masukkan harga produk: ");
    scanf("%f", &products[productCount].price);
    printf("Masukkan jumlah stok: ");
    scanf("%d", &products[productCount].stock);

    productCount++;
    printf("Produk baru berhasil ditambahkan!\n");
}

// Fungsi untuk merestok produk
void restockProduct() {
    int productIndex, restockAmount;
    displayProducts();
    printf("Masukkan nomor produk yang ingin di-restock: ");
    scanf("%d", &productIndex);

    if (productIndex < 1 || productIndex > productCount) {
        printf("Nomor produk tidak valid.\n");
        return;
    }

    printf("Masukkan jumlah restock: ");
    scanf("%d", &restockAmount);
    products[productIndex - 1].stock += restockAmount;

    printf("Restock berhasil! Stok %s sekarang: %d\n", products[productIndex - 1].name, products[productIndex - 1].stock);
}

// Fungsi untuk mengurangi stok produk
void reduceProductStock() {
    int productIndex, reduceAmount;
    displayProducts();
    printf("Masukkan nomor produk yang ingin dikurangi stoknya: ");
    scanf("%d", &productIndex);

    if (productIndex < 1 || productIndex > productCount) {
        printf("Nomor produk tidak valid.\n");
        return;
    }

    printf("Masukkan jumlah pengurangan stok: ");
    scanf("%d", &reduceAmount);

    if (products[productIndex - 1].stock < reduceAmount) {
        printf("Pengurangan melebihi stok yang tersedia.\n");
    } else {
        products[productIndex - 1].stock -= reduceAmount;
        printf("Stok %s berhasil dikurangi. Stok sekarang: %d\n", products[productIndex - 1].name, products[productIndex - 1].stock);
    }
}

// Fungsi untuk menghapus produk
void deleteProduct() {
    int productIndex;
    displayProducts();
    printf("Masukkan nomor produk yang ingin dihapus: ");
    scanf("%d", &productIndex);

    if (productIndex < 1 || productIndex > productCount) {
        printf("Nomor produk tidak valid.\n");
        return;
    }

    // Geser produk dalam array untuk menghapus produk
    for (int i = productIndex - 1; i < productCount - 1; i++) {
        products[i] = products[i + 1];
    }
    productCount--;
    printf("Produk berhasil dihapus.\n");
}

// Fungsi checkout
void checkout() {
    if (cartItemCount == 0) {
        printf("Keranjang kosong. Tidak ada yang bisa di-checkout.\n");
        return;
    }

    displayCart();

    printf("\n=== Checkout ===\n");
    float total = 0, harga_ppn = 0;
    for (int i = 0; i < cartItemCount; i++) {
        total += cart[i].product.price * cart[i].quantity;
    }

    harga_ppn = total*11/100;
    harga_ppn = total + harga_ppn;

    printf("Total yang harus dibayar + ppn(11%): Rp %.2f\n", harga_ppn);

    // Pilihan pembayaran dengan jumlah lembar
    float paid = 0;
    while (paid < harga_ppn) {
        int paymentOption, quantity;
        printf("Daftar Pecahan Uang:\n");
        printf("1: Rp 100    6: Rp 5000\n");
        printf("2: Rp 200    7: Rp 10000\n");
        printf("3: Rp 500    8: Rp 20000\n");
        printf("4: Rp 1000   9: Rp 50000\n");
        printf("5: Rp 2000   10: Rp 100000\n");
        printf("Pilih pecahan uang: ");
        scanf("%d", &paymentOption);
        printf("Masukkan jumlah lembar: ");
        scanf("%d", &quantity);

        float amount = 0;
        switch (paymentOption) {
            case 1: amount = 100; break;
            case 2: amount = 200; break;
            case 3: amount = 500; break;
            case 4: amount = 1000; break;
            case 5: amount = 2000; break;
            case 6: amount = 5000; break;
            case 7: amount = 10000; break;
            case 8: amount = 20000; break;
            case 9: amount = 50000; break;
            case 10: amount = 100000; break;
            default:
                printf("Opsi tidak valid.\n");
                continue;
        }

        paid += amount * quantity;
        printf("Jumlah pembayaran saat ini: Rp %.2f\n", paid);
        if(paid < harga_ppn){
            printf("Pembayaran kurang: Rp %.2f\n", harga_ppn-paid);
        }
    }

    float change = paid - harga_ppn;
    printf("Pembayaran berhasil! Kembalian: Rp %.2f\n", change);

    // Cetak struk pembelian
    // Variabel untuk menyimpan data
    char toko[] = "PAZ MART";
    char alamat[] = "Jl. Raya ITS, Keputih, Surabaya, 60111";
    char no_telepon[] = "0812-3456-7890";
    time_t t;
    struct tm *timeInfo;
    time(&t);
    timeInfo = localtime(&t);
    mktime(timeInfo); // Konversi otomatis jika perubahan menyebabkan overflow

    // Menampilkan
    printf("\n\n\t\t============================================\n");
    printf("\t\t||                %s                ||\n", toko);
    printf("\t\t|| %s ||\n", alamat);
    printf("\t\t||             %s             ||\n", no_telepon);
    printf("\t\t============================================\n");
    printf("\t\tNama Pembeli: %s\n", currentCustomerName);
    printf("\t\tWaktu       : %02d-%02d-%d %02d:%02d:%02d\n", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    printf("\t\t--------------------------------------------\n");
    for (int i = 0; i < cartItemCount; i++) {
        printf("\t\t%-20s\n", cart[i].product.name);
        printf("\t\t%d X %-10.2f\t\t       Rp %10.2f\n", cart[i].quantity, cart[i].product.price, cart[i].product.price*cart[i].quantity);
    }
    printf("\t\t--------------------------------------------\n");
    printf("\t\tTotal (PPN 11%%)                Rp %10.2f\n", harga_ppn);
    printf("\t\tBayar                          Rp %10.2f\n", paid);
    printf("\t\tKembalian                      Rp %10.2f\n", change);
    printf("\t\tTerima kasih telah berbelanja di PAZ MART!\n\n\n");

    cartItemCount = 0; // Kosongkan keranjang setelah checkout
}

// Fungsi untuk melakukan sign-up
void signUp() {
    if (customerCount >= MAX_CUSTOMERS) {
        printf("Jumlah pelanggan maksimal tercapai. Tidak bisa menambah pelanggan baru.\n");
        return;
    }

    char nrp[15], name[50];
    printf("Masukkan NRP (hanya angka): ");
    scanf("%s", nrp);

    if (!isNumeric(nrp)) {
        printf("NRP harus berupa angka. Sign-up gagal.\n");
        return;
    }

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].nrp, nrp) == 0) {
            printf("NRP sudah terdaftar. Silakan login.\n");
            return;
        }
    }

    printf("Masukkan Nama: ");
    scanf(" %[^\n]", name);

    strcpy(customers[customerCount].nrp, nrp);
    strcpy(customers[customerCount].name, name);
    customerCount++;

    printf("Sign-up berhasil! Silakan login.\n");
}

// Fungsi untuk melakukan login
int login() {
    char nrp[15];
    printf("Masukkan NRP Anda: ");
    scanf("%s", nrp);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].nrp, nrp) == 0) {
            strcpy(currentCustomerName, customers[i].name);
            printf("Login berhasil! Selamat datang, %s.\n", currentCustomerName);
            return 1;
        }
    }

    printf("NRP tidak ditemukan. Silakan sign-up terlebih dahulu.\n");
    return 0;
}

// Fungsi utama untuk pembeli
void customerMenu() {
    int choice;

    while (1) {
        printf("\n=== Menu Pembeli ===\n");
        printf("1. Lihat Produk\n");
        printf("2. Tambah ke Keranjang\n");
        printf("3. Lihat Keranjang\n");
        printf("4. Checkout\n");
        printf("9. Keluar ke Menu Utama\n");
        printf("Pilih opsi: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayProducts();
                break;
            case 2:
                addToCart();
                break;
            case 3:
                displayCart();
                break;
            case 4:
                checkout();
                break;
            case 9:
                return;
            default:
                printf("Opsi tidak valid. Silakan coba lagi.\n");
        }
    }
}

// Fungsi untuk menampilkan menu karyawan
void employeeMenu() {
    int employeeChoice;
    while (1) {
        printf("\n=== Menu Karyawan ===\n");
        printf("1. Lihat Produk\n");
        printf("2. Restock Produk\n");
        printf("3. Tambah Produk Baru\n");
        printf("4. Kurangi Stok Produk\n");
        printf("5. Hapus Produk\n");
        printf("9. Kembali ke Menu Utama\n");
        printf("Pilih opsi: ");
        scanf("%d", &employeeChoice);

        switch (employeeChoice) {
            case 1:
                displayProducts();
                break;
            case 2:
                restockProduct();
                break;
            case 3:
                addNewProduct();
                break;
            case 4:
                reduceProductStock();
                break;
            case 5:
                deleteProduct();
                break;
            case 9:
                return; // Kembali ke menu utama
            default:
                printf("Opsi tidak valid. Silakan coba lagi.\n");
        }
    }
}


// Fungsi utama program
int main() {
    int mainChoice;

    while (1) {
        printf("\nSelamat datang di PAZ MART\n");
        printf("1. Masuk sebagai Pembeli\n");
        printf("2. Masuk sebagai Karyawan\n");
        printf("3. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &mainChoice);

        if (mainChoice == 1) {
            int loginChoice;
            while (1) {
                printf("\n=== Login atau Sign-Up ===\n");
                printf("1. Login\n");
                printf("2. Sign-Up\n");
                printf("3. Kembali ke Menu Utama\n");
                printf("Pilih opsi: ");
                scanf("%d", &loginChoice);

                if (loginChoice == 1) {
                    if (login()) {
                        customerMenu();
                        break;
                    }
                } else if (loginChoice == 2) {
                    signUp();
                } else if (loginChoice == 3) {
                    break;
                } else {
                    printf("Opsi tidak valid. Silakan coba lagi.\n");
                }
            }
        } else if (mainChoice == 2) {
            char code[10];
            printf("Masukkan Kode Karyawan: ");
            scanf("%s", code);

            if (strcmp(code, EMPLOYEE_CODE) == 0) {
                printf("Akses mode karyawan diterima.\n");
                displayProducts();
                employeeMenu();
            } else {
                printf("Kode salah. Kembali ke menu utama.\n");
            }
        } else if (mainChoice == 3) {
            printf("Terima kasih telah menggunakan PAZ MART. Sampai jumpa!\n");
            break;
        } else {
            printf("Opsi tidak valid. Silakan coba lagi.\n");
        }
    }

    return 0;
}

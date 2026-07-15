#include <iostream>
#include <windows.h>
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"

namespace {
    const std::string SAMPLES_PATH = "data/samples.json";
    const std::string ORDERS_PATH  = "data/orders.json";

    void printSamples(SampleRepository& repo) {
        auto list = repo.findAll();
        if (list.empty()) { std::cout << "  (없음)\n"; return; }
        for (const auto& s : list)
            std::cout << "  [" << s.id << "] " << s.name
                      << " | 재고:" << s.stock
                      << " | 수율:" << s.yieldRate
                      << " | 생산시간:" << s.avgProductionTime << "min\n";
    }

    void printOrders(OrderRepository& repo) {
        auto list = repo.findAll();
        if (list.empty()) { std::cout << "  (없음)\n"; return; }
        for (const auto& o : list)
            std::cout << "  [" << o.orderId << "] " << o.customer
                      << " | 시료:" << o.sampleId
                      << " | 수량:" << o.quantity
                      << " | 상태:" << o.status << "\n";
    }

    void runSampleMenu(SampleRepository& repo) {
        std::cout << "\n=== 시료 CRUD ===\n";
        std::cout << "[1] 전체조회  [2] ID조회  [3] 등록  [4] 수정  [5] 삭제  [0] 뒤로\n> ";
        int choice; std::cin >> choice;

        if (choice == 1) {
            std::cout << "\n-- 전체 시료 목록 --\n";
            printSamples(repo);

        } else if (choice == 2) {
            std::string id;
            std::cout << "ID 입력: "; std::cin >> id;
            auto s = repo.findById(id);
            if (s) std::cout << "  찾음: " << s->name << " 재고:" << s->stock << "\n";
            else   std::cout << "  ID [" << id << "] 없음\n";

        } else if (choice == 3) {
            Sample s;
            std::cout << "ID: "; std::cin >> s.id;
            std::cin.ignore();
            std::cout << "이름: "; std::getline(std::cin, s.name);
            std::cout << "평균생산시간(min): "; std::cin >> s.avgProductionTime;
            std::cout << "수율(0~1): "; std::cin >> s.yieldRate;
            std::cout << "초기재고: "; std::cin >> s.stock;
            repo.save(s);
            std::cout << "  등록 완료: " << s.id << "\n";

        } else if (choice == 4) {
            std::string id;
            std::cout << "수정할 ID: "; std::cin >> id;
            auto s = repo.findById(id);
            if (!s) { std::cout << "  없음\n"; return; }
            std::cout << "새 재고 수량: "; std::cin >> s->stock;
            repo.update(*s);
            std::cout << "  수정 완료\n";

        } else if (choice == 5) {
            std::string id;
            std::cout << "삭제할 ID: "; std::cin >> id;
            std::cout << (repo.remove(id) ? "  삭제 완료\n" : "  없음\n");
        }
    }

    void runOrderMenu(OrderRepository& repo) {
        std::cout << "\n=== 주문 CRUD ===\n";
        std::cout << "[1] 전체조회  [2] ID조회  [3] 등록  [4] 상태변경  [5] 삭제  [0] 뒤로\n> ";
        int choice; std::cin >> choice;

        if (choice == 1) {
            std::cout << "\n-- 전체 주문 목록 --\n";
            printOrders(repo);

        } else if (choice == 2) {
            std::string id;
            std::cout << "주문ID: "; std::cin >> id;
            auto o = repo.findById(id);
            if (o) std::cout << "  찾음: " << o->customer << " 상태:" << o->status << "\n";
            else   std::cout << "  ID [" << id << "] 없음\n";

        } else if (choice == 3) {
            Order o;
            std::cout << "주문ID: "; std::cin >> o.orderId;
            std::cout << "시료ID: "; std::cin >> o.sampleId;
            std::cin.ignore();
            std::cout << "고객명: "; std::getline(std::cin, o.customer);
            std::cout << "수량: "; std::cin >> o.quantity;
            o.status    = "RESERVED";
            o.createdAt = "2026-07-15T00:00:00";
            repo.save(o);
            std::cout << "  주문 등록: " << o.orderId << "\n";

        } else if (choice == 4) {
            std::string id, status;
            std::cout << "주문ID: "; std::cin >> id;
            auto o = repo.findById(id);
            if (!o) { std::cout << "  없음\n"; return; }
            std::cout << "새 상태(RESERVED/CONFIRMED/PRODUCING/RELEASE/REJECTED): ";
            std::cin >> status;
            o->status = status;
            repo.update(*o);
            std::cout << "  상태 변경 완료\n";

        } else if (choice == 5) {
            std::string id;
            std::cout << "삭제할 주문ID: "; std::cin >> id;
            std::cout << (repo.remove(id) ? "  삭제 완료\n" : "  없음\n");
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SampleRepository sampleRepo(SAMPLES_PATH);
    OrderRepository  orderRepo(ORDERS_PATH);

    std::cout << "=== DataPersistence PoC — JSON CRUD 검증 ===\n";
    std::cout << "데이터 위치: data/samples.json, data/orders.json\n";

    while (true) {
        std::cout << "\n[1] 시료 관리  [2] 주문 관리  [0] 종료\n> ";
        int menu; std::cin >> menu;
        if (menu == 0) break;
        if (menu == 1) runSampleMenu(sampleRepo);
        if (menu == 2) runOrderMenu(orderRepo);
    }

    std::cout << "종료합니다.\n";
    return 0;
}

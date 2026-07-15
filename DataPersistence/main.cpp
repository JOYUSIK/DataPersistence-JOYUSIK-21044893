#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <functional>
#include <ctime>
#include <windows.h>
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"

static const int         COL_WIDTH  = 60;
static const std::string SEP(COL_WIDTH, '=');
static const std::string DIV(COL_WIDTH, '-');

// ─── Sample 메뉴 ────────────────────────────────────────────
static void listSamples(SampleRepository& repo) {
    auto list = repo.findAll();
    std::cout << "\n" << SEP << "\n";
    std::cout << " 시료 목록 (총 " << list.size() << "개)\n" << SEP << "\n";
    if (list.empty()) { std::cout << " 등록된 시료가 없습니다.\n"; return; }
    std::cout << " " << std::left << std::setw(8)  << "ID"
              << std::setw(20) << "이름"
              << std::setw(8)  << "재고"
              << std::setw(8)  << "수율"
              << "생산시간(min)\n";
    std::cout << DIV << "\n";
    for (const auto& s : list) {
        std::cout << " " << std::left << std::setw(8)  << s.id
                  << std::setw(20) << s.name
                  << std::setw(8)  << s.stock
                  << std::setw(8)  << s.yieldRate
                  << s.avgProductionTime << "\n";
    }
}

static void addSample(SampleRepository& repo) {
    std::cout << "\n" << SEP << "\n 시료 등록\n" << SEP << "\n";
    Sample s;
    std::cout << " 이름            > "; std::getline(std::cin, s.name);
    std::string buf;
    std::cout << " 평균생산시간(min) > "; std::getline(std::cin, buf);
    s.avgProductionTime = buf.empty() ? 0.0 : std::stod(buf);
    std::cout << " 수율 (0~1)       > "; std::getline(std::cin, buf);
    s.yieldRate = buf.empty() ? 1.0 : std::stod(buf);
    std::cout << " 초기재고         > "; std::getline(std::cin, buf);
    s.stock = buf.empty() ? 0 : std::stoi(buf);

    if (s.name.empty()) { std::cout << "\n [오류] 이름은 필수입니다.\n"; return; }
    Sample saved = repo.save(s);
    std::cout << "\n [완료] 시료가 등록되었습니다. (ID: " << saved.id << ")\n";
}

static void findSample(SampleRepository& repo) {
    std::cout << "\n 조회할 시료 ID > ";
    std::string id; std::getline(std::cin, id);
    auto s = repo.findById(id);
    if (!s) { std::cout << "\n [오류] 존재하지 않는 ID입니다: " << id << "\n"; return; }
    std::cout << "\n" << SEP << "\n 시료 상세\n" << SEP << "\n";
    std::cout << " ID           : " << s->id                << "\n";
    std::cout << " 이름         : " << s->name              << "\n";
    std::cout << " 재고         : " << s->stock             << "\n";
    std::cout << " 수율         : " << s->yieldRate         << "\n";
    std::cout << " 평균생산시간 : " << s->avgProductionTime << " min\n";
}

static void updateSampleStock(SampleRepository& repo) {
    std::cout << "\n 수정할 시료 ID > ";
    std::string id; std::getline(std::cin, id);
    auto s = repo.findById(id);
    if (!s) { std::cout << "\n [오류] 존재하지 않는 ID입니다: " << id << "\n"; return; }
    std::string buf;
    std::cout << " 현재 재고: " << s->stock << "\n";
    std::cout << " 새 재고 > "; std::getline(std::cin, buf);
    if (!buf.empty()) s->stock = std::stoi(buf);
    repo.update(*s);
    std::cout << "\n [완료] 재고가 수정되었습니다.\n";
}

static void removeSample(SampleRepository& repo) {
    std::cout << "\n 삭제할 시료 ID > ";
    std::string id; std::getline(std::cin, id);
    std::cout << (repo.remove(id) ? "\n [완료] 삭제되었습니다.\n"
                                  : "\n [오류] 존재하지 않는 ID입니다.\n");
}

// ─── Order 메뉴 ─────────────────────────────────────────────
static void listOrders(OrderRepository& repo) {
    auto list = repo.findAll();
    std::cout << "\n" << SEP << "\n";
    std::cout << " 주문 목록 (총 " << list.size() << "개)\n" << SEP << "\n";
    if (list.empty()) { std::cout << " 등록된 주문이 없습니다.\n"; return; }
    std::cout << " " << std::left << std::setw(22) << "주문ID"
              << std::setw(8)  << "시료ID"
              << std::setw(16) << "고객"
              << std::setw(8)  << "수량"
              << "상태\n";
    std::cout << DIV << "\n";
    for (const auto& o : list) {
        std::cout << " " << std::left << std::setw(22) << o.orderId
                  << std::setw(8)  << o.sampleId
                  << std::setw(16) << o.customer
                  << std::setw(8)  << o.quantity
                  << o.status << "\n";
    }
}

static void addOrder(OrderRepository& repo) {
    std::cout << "\n" << SEP << "\n 주문 등록\n" << SEP << "\n";
    Order o;
    std::cout << " 시료 ID > "; std::getline(std::cin, o.sampleId);
    std::cout << " 고객명  > "; std::getline(std::cin, o.customer);
    std::string buf;
    std::cout << " 수량    > "; std::getline(std::cin, buf);
    o.quantity  = buf.empty() ? 0 : std::stoi(buf);
    o.status = "RESERVED";
    std::time_t now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);
    char timeBuf[20];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%dT%H:%M:%S", &tm);
    o.createdAt = timeBuf;

    if (o.sampleId.empty() || o.customer.empty()) {
        std::cout << "\n [오류] 시료 ID와 고객명은 필수입니다.\n"; return;
    }
    Order saved = repo.save(o);
    std::cout << "\n [완료] 주문이 등록되었습니다. (주문ID: " << saved.orderId << ")\n";
}

static void findOrder(OrderRepository& repo) {
    std::cout << "\n 조회할 주문ID > ";
    std::string id; std::getline(std::cin, id);
    auto o = repo.findById(id);
    if (!o) { std::cout << "\n [오류] 존재하지 않는 주문ID입니다: " << id << "\n"; return; }
    std::cout << "\n" << SEP << "\n 주문 상세\n" << SEP << "\n";
    std::cout << " 주문ID   : " << o->orderId   << "\n";
    std::cout << " 시료ID   : " << o->sampleId  << "\n";
    std::cout << " 고객     : " << o->customer  << "\n";
    std::cout << " 수량     : " << o->quantity  << "\n";
    std::cout << " 상태     : " << o->status    << "\n";
    std::cout << " 등록일시 : " << o->createdAt << "\n";
}

static void updateOrderStatus(OrderRepository& repo) {
    std::cout << "\n 주문ID > ";
    std::string id; std::getline(std::cin, id);
    auto o = repo.findById(id);
    if (!o) { std::cout << "\n [오류] 존재하지 않는 주문ID입니다.\n"; return; }
    std::cout << " 현재 상태: " << o->status << "\n";
    std::cout << " 새 상태 (RESERVED/CONFIRMED/PRODUCING/RELEASE/REJECTED) > ";
    std::string status; std::getline(std::cin, status);
    if (!status.empty()) o->status = status;
    repo.update(*o);
    std::cout << "\n [완료] 상태가 변경되었습니다.\n";
}

static void removeOrder(OrderRepository& repo) {
    std::cout << "\n 삭제할 주문ID > ";
    std::string id; std::getline(std::cin, id);
    std::cout << (repo.remove(id) ? "\n [완료] 삭제되었습니다.\n"
                                  : "\n [오류] 존재하지 않는 주문ID입니다.\n");
}

using ActionMap = std::unordered_map<std::string, std::function<void()>>;

static void runSubMenu(const std::string& title, const ActionMap& actions) {
    std::cout << "\n" << SEP << "\n " << title << "\n" << SEP << "\n";
    std::cout << " [1] 전체조회  [2] ID조회  [3] 등록  [4] 수정  [5] 삭제  [0] 뒤로\n > ";
    std::string sub; std::getline(std::cin, sub);
    if (sub == "0") return;
    auto it = actions.find(sub);
    if (it != actions.end()) it->second();
    else std::cout << "\n [오류] 올바른 번호를 입력해주세요.\n";
}

// ─── 메인 ────────────────────────────────────────────────────
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SampleRepository sampleRepo;
    OrderRepository  orderRepo;

    const ActionMap sampleActions = {
        {"1", [&]{ listSamples(sampleRepo);      }},
        {"2", [&]{ findSample(sampleRepo);        }},
        {"3", [&]{ addSample(sampleRepo);         }},
        {"4", [&]{ updateSampleStock(sampleRepo); }},
        {"5", [&]{ removeSample(sampleRepo);      }},
    };
    const ActionMap orderActions = {
        {"1", [&]{ listOrders(orderRepo);       }},
        {"2", [&]{ findOrder(orderRepo);         }},
        {"3", [&]{ addOrder(orderRepo);          }},
        {"4", [&]{ updateOrderStatus(orderRepo); }},
        {"5", [&]{ removeOrder(orderRepo);       }},
    };

    const std::unordered_map<std::string, std::pair<std::string, const ActionMap*>> menuMap = {
        {"1", {"시료 관리", &sampleActions}},
        {"2", {"주문 관리", &orderActions}},
    };

    while (true) {
        std::cout << "\n" << SEP << "\n DataPersistence PoC — JSON CRUD 검증\n" << SEP << "\n";
        std::cout << " [1] 시료 관리\n [2] 주문 관리\n [0] 종료\n" << SEP << "\n 선택 > ";
        std::string menu; std::getline(std::cin, menu);
        if (menu == "0") { std::cout << "\n 시스템을 종료합니다.\n"; break; }
        auto it = menuMap.find(menu);
        if (it == menuMap.end()) { std::cout << "\n [오류] 올바른 메뉴 번호를 입력해주세요.\n"; continue; }
        runSubMenu(it->second.first, *it->second.second);
    }
    return 0;
}

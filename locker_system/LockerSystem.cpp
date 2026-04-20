#include <cmath>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// --------------------  ENUM -----------------------
enum class LockerSize { SMALL, MEDIUM, LARGE, XL, XXL };
enum class LockerState { AVAILABLE, OCCUPIED, EXPIRED };

// --------------------- Item ------------------------
class Item {
public:
    Item(string id, string name, LockerSize size, bool lockerEligible)
        : id_(std::move(id)),
          name_(std::move(name)),
          size_(size),
          lockerEligible_(lockerEligible) {}

    const string& getId() const { return id_; }
    const string& getName() const { return name_; }
    LockerSize getSize() const { return size_; }
    bool isLockerEligible() const { return lockerEligible_; }

private:
    string id_;
    string name_;
    LockerSize size_;
    bool lockerEligible_;
};

// -------------------- Customer ----------------------
class Customer {
public:
    Customer(string id, string name, string email) : id_(id), name_(name), email_(email) {}

    const string& getId() const { return id_; }
    const string& getName() const { return name_; }
    const string& getEmail() const { return email_; }

private:
    string id_;
    string name_;
    string email_;
};

// --------------------------------------------------------
// NotificationServeice: Abstract Interface
// --------------------------------------------------------
class NotificationService {
public:
    virtual void sendCode(const Customer& customer, const string& code) const = 0;
    virtual ~NotificationService() = default;
};

class ConsoleNotification : public NotificationService {
public:
    void sendCode(const Customer& customer, const string& code) const override {
        cout << "[Notification] Sending code " << code << " to " << customer.getName() << " ("
             << customer.getEmail() << ")\n";
    }
};

// ----------------------- Locker ---------------------------
class Locker {
public:
    Locker(int id, LockerSize size) : id_(id), size_(size), state_(LockerState::AVAILABLE) {}

    int getId() const { return id_; }
    LockerSize getSize() const { return size_; }
    LockerState getState() const { return state_; }
    bool isAvailable() const { return state_ == LockerState::AVAILABLE; }
    const string& getCode() const { return code_; }

    void occupy(const string& code) {
        code_ = code;
        state_ = LockerState::OCCUPIED;
    }

    void release() {
        code_ = "";
        state_ = LockerState::AVAILABLE;
    }

    void markExpired() { state_ = LockerState::EXPIRED; }

    bool canFitSize(LockerSize itemSize) const {
        return static_cast<int>(size_) >= static_cast<int>(itemSize);
    }

    void displayInfo() const {
        cout << "Locker #" << id_ << " [" << lockerSizeToString() << "]"
             << " State: "
             << (state_ == LockerState::AVAILABLE  ? "Available"
                 : state_ == LockerState::OCCUPIED ? "Occupited"
                                                   : "Expired")
             << "\n";
    }

private:
    int id_;
    LockerSize size_;
    LockerState state_;
    string code_;

    string lockerSizeToString() const {
        switch (size_) {
            case LockerSize::SMALL:
                return "Small";
            case LockerSize::MEDIUM:
                return "Medium";
            case LockerSize::LARGE:
                return "Large";
            case LockerSize::XL:
                return "XL";
            case LockerSize::XXL:
                return "XXL";
            default:
                return "Unknown";
        }
    }
};

// -------------------------- Reservation -----------------------------
struct Reservation {
    string customerId;
    string itemId;
    int lockerId;
    string code;
    time_t assignedAt;
    bool pickedUp{false};

    // bool isExpired() { return difftime(time(nullptr), assignedAt) > 3 * 24 * 3600; }
};

// ---------------------- CodeGenerator ------------------------------
class CodeGenerator {
public:
    static string generate6DigitCode() {
        static mt19937 rng(random_device{}());
        uniform_int_distribution<int> dist(100000, 999999);
        return to_string(dist(rng));
    }
};

// ----------------------- LockerLocation ------------------------------
class LockerLocation {
public:
    LockerLocation(int id, string name, double x, double y) : id_(id), name_(name), x_(x), y_(y) {}

    int getId() const { return id_; }
    const string& getName() const { return name_; }
    double getX() const { return x_; }
    double getY() const { return y_; }

    void addLocker(unique_ptr<Locker> locker) {
        availableLockers_[locker->getSize()].push(locker.get());
        lockers_.push_back(move(locker));
    }

    double distanceTo(double x, double y) const { return sqrt(pow(x_ - x, 2) + pow(y_ - y, 2)); }

    Locker* findAvailableLocker(LockerSize size) const {
        for (auto s = static_cast<int>(size); s <= static_cast<int>(LockerSize::XXL); s++) {
            auto it = availableLockers_.find(static_cast<LockerSize>(s));
            if (it != availableLockers_.end() && !it->second.empty()) return it->second.front();
        }
        return nullptr;
    }

    void occupyLocker(Locker* locker, const string& code) {
        if (!locker) return;
        auto& q = availableLockers_[locker->getSize()];
        q.pop();
        locker->occupy(code);
    }

    void releaseLocker(Locker* locker) {
        locker->release();
        availableLockers_[locker->getSize()].push(locker);
    }

    Locker* findLockerById(int id) const {
        for (const auto& locker : lockers_) {
            if (locker->getId() == id) return locker.get();
        }
        return nullptr;
    }

    bool hasAvailableLockerForSize(LockerSize size) const {
        return findAvailableLocker(size) != nullptr;
    }

    void displayInfo() const {
        cout << "Location: " << name_ << " (" << x_ << ", " << y_ << ")\n";
        for (const auto& locker : lockers_) locker->displayInfo();
    }

private:
    int id_;
    string name_;
    double x_, y_;
    vector<unique_ptr<Locker>> lockers_;
    mutable unordered_map<LockerSize, queue<Locker*>> availableLockers_;
};

// --------------------- LockerSystem -------------------------
class LockerSystem {
public:
    explicit LockerSystem(NotificationService& notifier) : notifier_(notifier) {}

    void addLocation(unique_ptr<LockerLocation> location) { locations_.push_back(move(location)); }

    // 1. Assign Locker
    bool assignLocker(const Customer& customer, const Item& item, double customerX,
                      double customerY) {
        if (!item.isLockerEligible()) {
            cout << "[Error] '" << item.getName() << "' is not locker eligible.\n";
            return false;
        }

        if (reservations_.count(customer.getId())) {
            cout << "[Error] " << customer.getName() << " already has an active reservation.\n";
            return false;
        }

        LockerLocation* location = findNearestLocation(customerX, customerY, item.getSize());
        if (!location) {
            cout << "[Error] No available locker near (" << customerX << ", " << customerY
                 << ").\n";
            return false;
        }

        Locker* locker = location->findAvailableLocker(item.getSize());
        if (!locker) return false;

        string code = CodeGenerator::generate6DigitCode();
        location->occupyLocker(locker, code);

        Reservation res;
        res.customerId = customer.getId();
        res.itemId = item.getId();
        res.lockerId = locker->getId();
        res.code = code;
        res.assignedAt = time(nullptr);
        reservations_[customer.getId()] = res;

        lockerLocationMap_[locker->getId()] = location;

        notifier_.sendCode(customer, code);
        cout << "[Assigned] '" << item.getName() << "' -> locker #" << locker->getId() << " at "
             << location->getName() << "\n";
        return true;
    }

    bool pickUp(const Customer& customer, const string& inputCode) {
        auto it = reservations_.find(customer.getId());
        if (it == reservations_.end()) {
            cout << "[Error] No reservation for " << customer.getName() << "\n";
            return false;
        }

        Reservation& res = it->second;

        if (res.pickedUp) {
            cout << "[Error] Already picked up.\n";
            return false;
        }

        if (res.code != inputCode) {
            cout << "[Error] Wrong code for " << customer.getName() << "\n";
            return false;
        }

        LockerLocation* location = lockerLocationMap_[res.lockerId];
        Locker* locker = location->findLockerById(res.lockerId);
        location->releaseLocker(locker);

        res.pickedUp = true;
        cout << "[PickedUp] " << customer.getName() << " picked up from Locker #" << res.lockerId
             << "\n";

        reservations_.erase(it);
        lockerLocationMap_.erase(res.lockerId);
        return true;
    }

private:
    LockerLocation* findNearestLocation(double x, double y, LockerSize size) const {
        LockerLocation* nearest = nullptr;
        double minDist = numeric_limits<double>::max();
        for (const auto& loc : locations_) {
            if (loc->hasAvailableLockerForSize(size)) {
                double dist = loc->distanceTo(x, y);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = loc.get();
                }
            }
        }

        return nearest;
    }

    vector<unique_ptr<LockerLocation>> locations_;
    unordered_map<string, Reservation> reservations_;
    unordered_map<int, LockerLocation*> lockerLocationMap_;  // lockerId -> which location
    NotificationService& notifier_;
};

int main() {}
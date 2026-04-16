#pragma once

#include "Models.hpp"
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <span>
#include <ranges>
#include <print>

/* 
--- Layout ---
--- Manage arrangement and index of all seats in some cinema ---
*/
class Layout {
private:
    int rows;
    int columns;
    std::map<std::string, std::shared_ptr<Seat>> seatsByNumber;
    std::map<int, std::map<int, std::shared_ptr<Seat>>> seatsByPosition;

public:
    Layout(int r, int c): rows(r), columns(c) {}
    ~Layout() {}

    void addSeat(std::string seatNum, int row, int col, std::unique_ptr<PricingStrategy> stratege)
    {
        auto seat = std::make_shared<Seat>(seatNum, std::move(stratege));
        seatsByNumber[seatNum] = seat;
        seatsByPosition[row][col] = seat;
    }

    std::shared_ptr<Seat> getSeatByNumber(std::string_view num) const
    {
        auto it = seatsByNumber.find(std::string(num)); 
        if (it != seatsByNumber.end())
        {
            return it->second;
        }
        return nullptr;
    }

    std::shared_ptr<Seat> getSeatByPosition(int row, int col) const
    {
        auto it = seatsByPosition.find(row);
        if (it != seatsByPosition.end())
        {
            auto it2 = it->second.find(col);
            if (it2 != it->second.end())
            {
                return it2->second;
            }
        }
        return nullptr;
    }

    auto getAllSeats() const
    {
        return std::views::values(seatsByNumber);
    }

};
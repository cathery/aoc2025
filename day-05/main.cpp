#include <cstddef>
#include <cstdio>
#include <fstream>
#include <vector>

class TRangeSet
{
    struct TRange
    {
        size_t From;
        size_t To;
    };

    std::vector<TRange> _Ranges;

public:
    void AddRange(size_t From, size_t To)
    {
        if (_Ranges.empty())
        {
            _Ranges.push_back({From, To});
            return;
        }

        for (auto It = _Ranges.begin(); It != _Ranges.end(); ++It)
        {
            const auto& RangeFrom = It->From;
            const auto& RangeTo = It->To;

            // if we start before this range
            if (From < RangeFrom)
            {
                // and we end before this range
                if (To < RangeFrom)
                {
                    // ranges are unrelated
                    continue;
                }
                // and we end within this range
                else if (To >= RangeFrom && To <= RangeTo)
                {
                    // end is inside range, extend start
                    It->From = From;
                    return;
                }
                // and we end after this range
                else if (To > RangeTo)
                {
                    // we completely envelop this range, extend start and end
                    It->From = From;
                    It->To = To;

                    // if this makes us overlap with the next range, merge ranges and remove the copy
                    auto NextIt = It+1;
                    if (NextIt->From < To)
                    {
                        It->To = NextIt->To;
                        _Ranges.erase(NextIt);
                    }
                    return;
                }
            }
            // if we start within this range
            else if (From >= RangeFrom && From <= RangeTo)
            {
                // and we end within this range
                if (To <= RangeTo)
                {
                    // we are completely enveloped by this range, no action needed
                    return;
                }
                // and we end after this range
                else
                {
                    // extend its end with ours
                    It->To = To;

                    // if this makes us overlap with the next range, merge ranges and remove the copy
                    auto NextIt = It+1;
                    if (NextIt->From < To)
                    {
                        It->To = NextIt->To;
                        _Ranges.erase(NextIt);
                    }
                    return;
                }
            }
            // if we start after this range
            else if (From > RangeTo)
            {
                // ranges are unrelated
                continue;
            }
        }

        //if we ended up here, we're unrelated to any ranges, and may add this one to the list
        for (auto It = _Ranges.begin(); It != _Ranges.end(); ++It)
        {
            if (From > It->To)
            {
                continue;
            }

            if (To < It->From)
            {
                // insert the range between the lesser and the bigger range
                _Ranges.insert(It, {From, To});
                return;
            }
        }

        // if we are larger than any other range, add it to the very end
        _Ranges.push_back({From, To});
    }

    size_t CalculateNumValues() const
    {
        size_t NumValues = 0;
        for (const auto& [From, To] : _Ranges)
        {
            NumValues += To - From + 1;
        }

        return NumValues;
    }

    bool IsWithinRange(size_t Number) const
    {
        for (const auto& [From, To] : _Ranges)
        {
            if (Number >= From && Number <= To)
            {
                return true;
            }
        }
        return false;
    }
};

int main()
{
    std::ifstream Input("input.txt");
    size_t Part1Answer = 0;
    size_t Part2Answer = 0;

    TRangeSet RangeSet;

    while (Input.peek() != '\n')
    {
        size_t RangeStart;
        size_t RangeEnd;

        Input >> RangeStart;
        Input.ignore(1);
        Input >> RangeEnd;
        Input.ignore(1);

        RangeSet.AddRange(RangeStart, RangeEnd);
    }

    while (Input.peek() != EOF)
    {
        size_t IngredientID;
        Input >> IngredientID;
        Input.ignore(1);

        Part1Answer += RangeSet.IsWithinRange(IngredientID);
    }

    Part2Answer = RangeSet.CalculateNumValues();

    printf("Part 1 answer: %lld\n", Part1Answer);
    printf("Part 2 answer: %lld\n", Part2Answer);
}
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <fstream>

size_t GetInvalidID(size_t ID, bool& OutIsPart1)
{
    if (ID < 10)
    {
        return 0;
    }

    const size_t NumDigits = std::log10(ID) + 1;
    for (size_t NumPatternDigits = NumDigits / 2; NumPatternDigits > 0; --NumPatternDigits)
    {
        if (NumDigits % NumPatternDigits != 0)
        {
            continue;
        }

        const size_t Divisor = std::pow(10, NumPatternDigits);
        size_t Pattern = ID % Divisor;
        size_t IDRemainder = ID / Divisor;

        if (Pattern == IDRemainder)
        {
            OutIsPart1 = true;
            return ID;
        }

        while (IDRemainder > 0) 
        {
            if (Pattern != IDRemainder % Divisor)
            {
                break;
            }

            IDRemainder /= Divisor;
        }

        if (IDRemainder == 0)
        {
            OutIsPart1 = false;
            return ID;
        }
    }

    return 0;
}
int main()
{
    std::ifstream Input("input.txt");
    size_t Part1Answer = 0;
    size_t Part2Answer = 0;

    while (Input.peek() != EOF)
    {
        size_t FirstID;
        size_t LastID;

        Input >> FirstID;
        Input.ignore();
        Input >> LastID;
        Input.ignore();

        for (size_t ID = FirstID; ID <= LastID; ++ID)
        {
            bool IsPart1;
            const size_t InvalidID = GetInvalidID(ID, IsPart1);
            Part1Answer += InvalidID * IsPart1;
            Part2Answer += InvalidID;
        }

    }

    std::printf("Part 1 answer: %lld\n", Part1Answer);
    std::printf("Part 2 answer: %lld\n", Part2Answer);
}
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>

int main()
{
    std::ifstream Input("input.txt");
    size_t Part1Answer = 0;
    size_t Part2Answer = 0;

    // Numbers[Y][X] = number
    // Digits[Y][X] = digit
    std::vector<std::vector<size_t>> Part1Numbers;
    std::vector<std::vector<uint8_t>> Part2Digits;

    // Parse all digits into a grid
    while (!ispunct(Input.peek()))
    {
        auto& Part1Line = Part1Numbers.emplace_back();
        auto& Part2Line = Part2Digits.emplace_back();

        size_t Part1Number = 0;
        while (true)
        {
            const char Char = Input.get();
            const bool bIsDigit = isdigit(Char) != 0;
            const uint8_t Digit = Char - '0';

            if (bIsDigit)
            {
                Part1Number *= 10;
                Part1Number += Digit;
            }
            else if (Part1Number != 0)
            {
                Part1Line.push_back(Part1Number);
                Part1Number = 0;
            }

            if (Char == '\n')
            {
                break;
            }

            Part2Line.push_back(Digit * bIsDigit);
        }
    }

    // Parse all operators and add up numbers during it
    bool bMultiplication = false;
    size_t Part1Column = 0;
    size_t Part2Result = 0;
    for (size_t XIndex = 0;; ++XIndex)
    {
        const char Char = Input.get();

        // if char is end of line/file
        if (iscntrl(Char))
        {
            Part2Answer += Part2Result;
            break;
        }

        // If char is an operator
        if (ispunct(Char))
        {
            Part2Answer += Part2Result;

            bMultiplication = Char == '*';
            Part2Result = bMultiplication;

            // Use the operator to add up all part 1 numbers
            size_t Part1Result = bMultiplication;
            for (const auto& Line : Part1Numbers)
            {
                if (bMultiplication)
                {
                    Part1Result *= Line[Part1Column];
                }
                else
                {
                    Part1Result += Line[Part1Column];
                }
            }

            Part1Answer += Part1Result;
            ++Part1Column;
        }

        size_t Part2Number = 0;
        for (const auto& Line : Part2Digits)
        {
            if (const auto Digit = Line[XIndex])
            {
                Part2Number *= 10;
                Part2Number += Line[XIndex];
            }
        }

        if (Part2Number != 0)
        {
            if (bMultiplication)
            {
                Part2Result *= Part2Number;
            }
            else
            {
                Part2Result += Part2Number;
            }
        }
    }

    printf("Part 1 answer: %lld\n", Part1Answer);
    printf("Part 2 answer: %lld\n", Part2Answer);
}
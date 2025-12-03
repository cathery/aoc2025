#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <string>
#include <ranges>

struct TBattery
{
    size_t Index = 0;
    size_t Joltage = 0;
};

struct TPart1Sequence
{
    TBattery FirstBattery;
    TBattery SecondBattery;
};

struct TPart2Sequence
{
    std::array<TBattery, 12> Batteries;
};

int main()
{
    std::ifstream Input("input.txt");
    size_t Part1Answer = 0;
    size_t Part2Answer = 0;

    while (Input.peek() != EOF)
    {
        std::string Bank;
        Input >> Bank;
        Input.ignore(1);

        // Key is the first battery's joltage, value is the sequence
        std::array<TPart1Sequence, 9> Part1Sequences;
        std::array<TPart2Sequence, 9> Part2Sequences;

        // Read the bank
        for (size_t CurIndex = 0; CurIndex != Bank.size(); ++CurIndex)
        {
            TBattery CurBattery;
            CurBattery.Index = CurIndex;
            CurBattery.Joltage = Bank[CurIndex] - '0';

            // Place current battery in the sequence map
            if (Part1Sequences[CurBattery.Joltage-1].FirstBattery.Joltage == 0)
            {
                Part1Sequences[CurBattery.Joltage-1].FirstBattery = CurBattery;
                Part2Sequences[CurBattery.Joltage-1].Batteries[0] = CurBattery;
            }

            // For part 1
            for (TPart1Sequence& Sequence : Part1Sequences)
            {
                if (Sequence.FirstBattery.Joltage != 0
                    && CurBattery.Index > Sequence.FirstBattery.Index
                    && CurBattery.Joltage > Sequence.SecondBattery.Joltage)
                {
                    Sequence.SecondBattery = CurBattery;
                }
            }

            // For part 2
            for (TPart2Sequence& Sequence : Part2Sequences)
            {
                // If sequence is not valid, ignore
                if (Sequence.Batteries[0].Joltage == 0 || CurIndex <= Sequence.Batteries[0].Index)
                {
                    continue;
                }

                // Go through every battery in the sequence and check if current battery is a better fit
                for (size_t SequenceIndex = 0; SequenceIndex != Sequence.Batteries.size(); ++SequenceIndex)
                {
                    TBattery& SequenceBattery = Sequence.Batteries[SequenceIndex];
                    bool bHasRoomForFullSequence = (Bank.size() - CurIndex) >= (Sequence.Batteries.size() - SequenceIndex);

                    // If current joltage is higher than the current sequence, replace it
                    if (bHasRoomForFullSequence && CurBattery.Joltage > SequenceBattery.Joltage)
                    {
                        SequenceBattery = CurBattery;

                        // We just changed the entire sequence, empty all data after this index
                        std::fill(Sequence.Batteries.begin() + SequenceIndex+1, Sequence.Batteries.end(), TBattery{});
                        break;
                    }
                }
            }
        }

        // For part 1
        for (const TPart1Sequence& Sequence : Part1Sequences | std::views::reverse)
        {
            if (Sequence.FirstBattery.Joltage != 0 && Sequence.SecondBattery.Joltage != 0)
            {
                Part1Answer += Sequence.FirstBattery.Joltage * 10 + Sequence.SecondBattery.Joltage;
                break;
            }
        }

        // For part 2
        for (const TPart2Sequence& Sequence : Part2Sequences | std::views::reverse)
        {
            bool bValidSequence = true;
            size_t TotalJoltage = 0;

            for (const TBattery& Battery : Sequence.Batteries)
            {
                if (Battery.Joltage == 0)
                {
                    bValidSequence = false;
                    break;
                }

                TotalJoltage *= 10;
                TotalJoltage += Battery.Joltage;
            }

            if (bValidSequence)
            {
                Part2Answer += TotalJoltage;
                break;
            }
        }
    }

    printf("Part 1 answer: %lld\n", Part1Answer);
    printf("Part 2 answer: %lld\n", Part2Answer);
}
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream Input("input.txt");

    int32_t CurrentStepIndex = 50;
    int32_t TotalZeroHitsPart1 = 0;
    int32_t TotalZeroHitsPart2 = 0;

    char Direction;
    int32_t StepCount;
    while (Input.peek() != EOF)
    {
        while (Input.peek() != '\n' && Input.peek() != EOF)
        {
            Input >> Direction;
            Input >> StepCount;
            Input.ignore(1);

            TotalZeroHitsPart2 += StepCount / 100;
            StepCount %= 100;

            if (Direction == 'L')
            {
                StepCount = -StepCount;
            }

            const bool IsStartAtZero = CurrentStepIndex == 0;
            const bool IsOverEdge = CurrentStepIndex + StepCount < 1 || CurrentStepIndex + StepCount > 99;

            TotalZeroHitsPart2 += !IsStartAtZero * IsOverEdge;

            CurrentStepIndex = ((CurrentStepIndex + StepCount + 100) % 100);
            TotalZeroHitsPart1 += CurrentStepIndex == 0;
        }
    }

    printf("Part 1: %d\nPart 2: %d\n", TotalZeroHitsPart1, TotalZeroHitsPart2);
}
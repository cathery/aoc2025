#include <cstddef>
#include <cstdio>
#include <fstream>
#include <map>
#include <set>

int main()
{
    std::ifstream Input("input.txt");
    size_t Part1Answer = 0;
    size_t Part2Answer = 0;

    // Key is XIndex
    std::set<size_t> BeamXPositions;

    // Key is XIndex, Val is number of timelines
    std::map<size_t, size_t> BeamXTimelines;

    while (Input.peek() != EOF)
    {
        size_t XIndex = 0;
        while (true)
        {
            const char Char = Input.get();
            if (Char == '\n')
            {
                break;
            }
            else if (Char == 'S')
            {
                BeamXPositions.insert(XIndex);
                BeamXTimelines[XIndex] += 1;
            }
            else if (Char == '^')
            {
                if (bool bRemoved = BeamXPositions.erase(XIndex))
                {
                    BeamXPositions.insert(XIndex-1);
                    BeamXPositions.insert(XIndex+1);
                    ++Part1Answer;
                }

                size_t TimelineCount = BeamXTimelines[XIndex];
                BeamXTimelines[XIndex-1] += TimelineCount;
                BeamXTimelines[XIndex+1] += TimelineCount;
                BeamXTimelines.erase(XIndex);
            }

            ++XIndex;
        }
    }

    for (const auto& [XIndex, TimelineCount] : BeamXTimelines)
    {
        Part2Answer += TimelineCount;
    }

    printf("Part 1 answer: %lld\n", Part1Answer);
    printf("Part 2 answer: %lld\n", Part2Answer);
}
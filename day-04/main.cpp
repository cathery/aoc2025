#include <cstddef>
#include <cstdio>
#include <fstream>
#include <utility>
#include <vector>

int main()
{
    std::ifstream Input("input.txt");
    size_t Part1Answer = 0;
    size_t Part2Answer = 0;

    std::vector<std::vector<bool>> Grid;

    while (Input.peek() != EOF)
    {
        auto& Line = Grid.emplace_back();

        while (true)
        {
            const char Char = Input.get();

            if (Char == '\n')
            {
                break;
            }

            Line.push_back(Char == '@');
        }
    }

    const size_t YLast = Grid.size() - 1;
    const size_t XLast = Grid[0].size() - 1;

    bool bPart1Done = 0;
    std::vector<std::pair<size_t, size_t>> TilesToRemove;

    while (true)
    {
        TilesToRemove.clear();

        for (size_t Y = 0; Y <= YLast; ++Y)
        {
            for (size_t X = 0; X <= XLast; ++X)
            {
                if (Grid[Y][X] == false)
                {
                    continue;
                }

                const size_t NeighborCount = 0
                + (Y > 0     && X > 0     && Grid[Y-1][X-1])
                + (Y > 0                  && Grid[Y-1][X])
                + (Y > 0     && X < XLast && Grid[Y-1][X+1])
                + (X > 0                  && Grid[Y][X-1])
                + (X < XLast              && Grid[Y][X+1])
                + (Y < YLast && X > 0     && Grid[Y+1][X-1])
                + (Y < YLast              && Grid[Y+1][X])
                + (Y < YLast && X < XLast && Grid[Y+1][X+1]);

                if (NeighborCount < 4)
                {
                    TilesToRemove.push_back({Y,X});
                }
            }
        }

        if (TilesToRemove.empty())
        {
            break;
        }

        Part1Answer += TilesToRemove.size() * !bPart1Done;
        Part2Answer += TilesToRemove.size();
        bPart1Done = true;

        for (const auto& [Y, X] : TilesToRemove)
        {
            Grid[Y][X] = false;
        }
    }

    printf("Part 1 answer: %lld\n", Part1Answer);
    printf("Part 2 answer: %lld\n", Part2Answer);
}

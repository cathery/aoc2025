#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <limits>
#include <map>
#include <set>

std::map<intmax_t, std::set<intmax_t>> RedTiles;
std::map<intmax_t, std::map<intmax_t, bool>> BadTiles;
intmax_t XMax = 0;
intmax_t YMax = 0;

struct TPoint
{
    intmax_t X;
    intmax_t Y;
};

struct TTarget
{
    TPoint TargetPoint;
    intmax_t ClosestDistanceToTarget = std::numeric_limits<intmax_t>::max();
    TPoint ClosestPointToTarget;
};

intmax_t GetArea(TPoint First, TPoint Second)
{
    const auto XDiff = (First.X > Second.X)? (First.X - Second.X) : (Second.X - First.X);
    const auto YDiff = (First.Y > Second.Y)? (First.Y - Second.Y) : (Second.Y - First.Y);
    return (XDiff + 1) * (YDiff + 1);
}

intmax_t SqDistance(TPoint First, TPoint Second)
{
    const auto XDiff = Second.X - First.X;
    const auto YDiff = Second.Y - First.Y;
    return XDiff*XDiff + YDiff*YDiff;
}

void DrawOutline(TPoint From, TPoint To)
{
    const auto XDiff = To.X - From.X;
    const auto XSign = (XDiff > 0) - (XDiff < 0);
    const auto YDiff = To.Y - From.Y;
    const auto YSign = (YDiff > 0) - (YDiff < 0);

    for (intmax_t XIndex = From.X; XIndex != (To.X + XSign); XIndex += XSign)
    {
        BadTiles[XIndex].insert_or_assign(From.Y, false);
        BadTiles[XIndex].try_emplace(From.Y - XSign, true);
    }

    for (intmax_t YIndex = From.Y; YIndex != (To.Y + YSign); YIndex += YSign)
    {
        BadTiles[From.X].insert_or_assign(YIndex, false);
        BadTiles[From.X + YSign].try_emplace(YIndex, true);
    }
}

bool TestLine(TPoint From, TPoint To)
{
    const auto XDiff = To.X - From.X;
    const auto XSign = (XDiff > 0) - (XDiff < 0);
    const auto YDiff = To.Y - From.Y;
    const auto YSign = (YDiff > 0) - (YDiff < 0);

    for (intmax_t XIndex = From.X; XIndex != To.X; XIndex += XSign)
    {
        if (BadTiles[XIndex][From.Y])
        {
            return false;
        }
    }

    for (intmax_t YIndex = From.Y; YIndex != To.Y; YIndex += YSign)
    {
        if (BadTiles[From.X][YIndex])
        {
            return false;
        }
    }

    return true;
}

int main()
{
    std::ifstream Input("input.txt");
    intmax_t LargestAreaPart1 = 0;
    intmax_t LargestAreaPart2 = 0;

    TPoint FirstPoint{};
    TPoint LastPoint{};
    while (Input.peek() != EOF)
    {
        TPoint Point;
        Input >> Point.X;
        Input.ignore(1);
        Input >> Point.Y;
        Input.ignore(1);

        RedTiles[Point.X].insert(Point.Y);
        XMax = std::max(XMax, Point.X);
        YMax = std::max(YMax, Point.Y);

        if (FirstPoint.X == 0)
        {
            FirstPoint = Point;
        }
        if (LastPoint.X != 0)
        {
            DrawOutline(LastPoint, Point);
        }

        LastPoint = Point;
    }
    DrawOutline(LastPoint, FirstPoint);

    /* Part 1 begin */
    std::array<TTarget,4> TargetPoints{{
        {0,0},
        {XMax,0},
        {0, YMax},
        {XMax,YMax}}};

    for (const auto& [XIndex, Column] : RedTiles)
    {
        for (const auto& YIndex : Column)
        {
            const TPoint CurPoint{XIndex, YIndex};
            for (auto& Target : TargetPoints)
            {
                const auto Distance = SqDistance(CurPoint, Target.TargetPoint);
                if (Distance < Target.ClosestDistanceToTarget)
                {
                    Target.ClosestDistanceToTarget = Distance;
                    Target.ClosestPointToTarget = CurPoint;
                }
            }
        }
    }

    for (const auto& Target1 : TargetPoints)
    {
        for (const auto& Target2 : TargetPoints)
        {
            LargestAreaPart1 = std::max(LargestAreaPart1, GetArea(Target1.ClosestPointToTarget, Target2.ClosestPointToTarget));
        }
    }

    printf("Part 1 answer is %lld\n", LargestAreaPart1);
    /* Part 1 end */

    /* Part 2 begin */
    for (const auto& [XIndex1, Column1] : RedTiles)
    {
        for (const auto& YIndex1 : Column1)
        {
            const TPoint Point1{XIndex1, YIndex1};

            std::map<intmax_t, std::set<intmax_t>>::reverse_iterator XRevIt;
            for (XRevIt = RedTiles.rbegin(); XRevIt != RedTiles.rend(); ++XRevIt)
            {
                const auto& [XIndex2, Column2] = *XRevIt;

                if (XIndex1 >= XIndex2)
                {
                    break;
                }

                std::set<intmax_t>::reverse_iterator YRevIt;
                for (YRevIt = Column2.rbegin(); YRevIt != Column2.rend(); ++YRevIt)
                {
                    const auto& YIndex2 = *YRevIt;

                    const TPoint Point2{XIndex2, YIndex2};

                    const intmax_t Area = GetArea(Point1, Point2);
                    if (Area <= LargestAreaPart2)
                    {
                        continue;
                    }

                    if (TestLine(Point1, {XIndex1, YIndex2})
                    && TestLine(Point1, {XIndex2, YIndex1})
                    && TestLine({XIndex1, YIndex2}, Point2)
                    && TestLine({XIndex2, YIndex1}, Point2))
                    {
                        LargestAreaPart2 = Area;
                        printf("New largest area of %lld\n", LargestAreaPart2);
                    }
                }
            }
        }
    }

    printf("Part 2 answer is %lld\n", LargestAreaPart2);
    /* Part 2 end */
}
#pragma once
inline float CalculateScreenSpaceRow(const int row, const int screenHeight)
{
	return (1 - 2 * ((static_cast<float>(row) + 0.5f) / screenHeight));
}
inline float CalculateScreenSpaceColumn(const int col, const int screenWidth)
{
	return 2 * ((static_cast<float>(col) + 0.5f) / screenWidth) - 1;
}
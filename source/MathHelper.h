#pragma once
inline float CalculateScreenSpaceColumn(const int col, const int screenWidth) // x
{
	const float x = (static_cast<float>(col) + 0.5f) / static_cast<float>(screenWidth);
	return 2 * x - 1;
}
inline float CalculateScreenSpaceRow(const int row, const int screenHeight) // y
{
	const float y = (static_cast<float>(row) + 0.5f) / static_cast<float>(screenHeight);
	return (1 - 2 * y);
}
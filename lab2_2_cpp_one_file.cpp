//Системи реального часу.Лабораторна робота 2.2.Швидке перетворення Фур'є
//Кравчука Олександра ІВ - 72
#include <iostream>
#include <cinttypes>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
#include <Windows.h>
typedef std::complex<float> complexf;
#define SLEEP_TIME 900000

//прототипи функцій
void generate_signal(uint32_t n, float Wmax, uint32_t N, float step, float* t, float* signal);
void fast_fourier_transform(float* signal, uint32_t N, complexf* result);

void linspace(float* arr, float start, float end, uint32_t n);
float* linspace(float start, float end, uint32_t n);
float find_max_in_arr(float* arr, uint32_t n);
float find_min_in_arr(float* arr, uint32_t n);
void abs_complex_arr(complexf* arr, uint32_t n, float* result);

void draw_schedue(float* xarr, float* yarr, uint32_t n,
	uint32_t left, uint32_t top, uint32_t width, uint32_t heidth);

//####################################################
void fast_fourier_transform(complexf* a, uint32_t n) {
	if (n == 1)  return;
	uint32_t n_2 = n >> 1; //n_2 = n/2;
	uint32_t i, j;
	//a0, a1 - парні, непарні елементи масиву a відповідно
	complexf* a0 = new complexf[n_2];
	complexf* a1 = new complexf[n_2];
	for (i = 0, j = 0; i < n; i += 2, ++j) { //розділення
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	//рекурсія
	fast_fourier_transform(a0, n_2);
	fast_fourier_transform(a1, n_2);

	float ang = 2 * M_PI / n;
	complexf w(1), wn(cosf(ang), sinf(ang)); //WpN, W1N 
	for (i = 0; i < n_2; ++i) { //об'єднання
		a[i] = a0[i] + w * a1[i]; //операція "метелик"
		a[i + n_2] = a0[i] - w * a1[i];
		w *= wn;
	}
	delete[] a0; delete[] a1; a0 = nullptr; a1 = nullptr;
}

void fast_fourier_transform(float* signal, uint32_t N, complexf* result) {
	for (uint32_t i = 0; i < N; ++i)
		result[i] = complexf(signal[i]);
	fast_fourier_transform(result, N);

}

void generate_signal(uint32_t n, float Wmax, uint32_t N, float step,
	float* t, float* signal) {

	uint32_t i, j;
	float Wmax2 = Wmax * 2 * M_PI; //циклічна частота
	float A = 0.0f, fi = 0.0f, w = 0.0f, max_t;
	max_t = step * (N - 1);

	//volume(signal, N, 0.0f); //заповнення нулями
	linspace(t, 0, max_t, N);

	srand(time(0));
	for (i = 0; i < n; ++i) {
		A = (float)rand() / (float)RAND_MAX*0.5f+0.5f;
		fi = (float)rand() / (float)RAND_MAX * 2 * M_PI;
		w = Wmax2 / n * (i + 1);

		for (j = 0; j < N; ++j)
			signal[j] += A * sin(w * t[j] + fi);
	}
}



int main() {
	setlocale(LC_ALL, "Ru");
	
	uint32_t
		n = 8,  //к-сть гармонік  
		Wmax = 1200, //гранична частота
		N = 16384, //к-сть дискретних відліків
		i;
	uint32_t N2 = N / 2 + 1;
	float step = 0.0004, T; //step -крок часу

	float* t = new float[N];
	float* signal = new float[N]();
	complexf* fourier_transform_result = new complexf[N](); //N2
	float* ampliudes = new float[N2]; //N2

	generate_signal(n, Wmax, N, step, t, signal);
	draw_schedue(t, signal, N, 0, 10, 1080, 250);

	T = t[N - 1];
	float* freq = linspace(1.f / T, 0.5f / step, N2); //N2

	//#############################################################
	std::cout << "N = " << N << "\n";
	long timeBefore, timeDiff;

	timeBefore = GetTickCount();
	fast_fourier_transform(signal, N, fourier_transform_result);  //####
	timeDiff = GetTickCount() - timeBefore;
	std::cout << "time elapsed(швидке): " << timeDiff << " мс\n";
	//#############################################################

	abs_complex_arr(fourier_transform_result, N2, ampliudes);

	draw_schedue(freq, ampliudes, N2, 0, 300, 1080, 250); 

	delete[] t, delete[] signal; delete[] fourier_transform_result;
	delete[] ampliudes; delete[] freq;

	t = nullptr; signal = nullptr; fourier_transform_result = nullptr;
	ampliudes = nullptr; freq = nullptr;
	Sleep(SLEEP_TIME);

	return 0;
	
}

//###############################################################
//інші функції
void linspace(float* arr, float start, float end, uint32_t n) {
	float current = start;
	float step = (end - start) / ((float)n - 1);
	for (uint32_t i = 0; i < n; ++i) {
		arr[i] = current; current += step;

	}
}
float* linspace(float start, float end, uint32_t n) {
	float* arr = new float[n];
	float current = start;
	float step = (end - start) / ((float)n - 1);
	for (uint32_t i = 0; i < n; ++i) {
		arr[i] = current; current += step;

	}
	return arr;
}
float find_max_in_arr(float* arr, uint32_t n) {
	float max = arr[0];
	for (uint32_t i = 1; i < n; ++i)
		if (arr[i] > max) max = arr[i];
	return max;
}

float find_min_in_arr(float* arr, uint32_t n) {
	float min = arr[0];
	for (uint32_t i = 1; i < n; ++i)
		if (arr[i] < min) min = arr[i];
	return min;
}
void abs_complex_arr(complexf* arr, uint32_t n, float* result) {
	for (uint32_t i = 0; i < n; ++i)  result[i] = abs(arr[i]);
}



//###############################################################
//функції для малювання графіків в консоль
struct TransformParameters {
	float scaleX, scaleY, offsetX, offsetY;
};

float* calculate_values_of_fuction(float* xarr, uint32_t n, float(*func)(float)) {
	float* yarr = new float[n];
	for (uint32_t i = 0; i < n; ++i) yarr[i] = func(xarr[i]);
	return yarr;
}

TransformParameters calculate_draw_parameters(float* xarr, float* yarr, uint32_t n,
	uint32_t left, uint32_t top, uint32_t width, uint32_t heidth) {
	float minY, maxY;
	minY = find_min_in_arr(yarr, n);  maxY = find_max_in_arr(yarr, n);

	TransformParameters transParam;
	transParam.scaleX = (width) / (xarr[n - 1] - xarr[0]);
	transParam.offsetX = -xarr[0] + left / transParam.scaleX;
	transParam.scaleY = (heidth) / (maxY - minY)*(-1.f);
	transParam.offsetY = -maxY + top / transParam.scaleY;
	return  transParam;
}

void transform_to_screen_coords(float* xarr, float* yarr, uint32_t n,
	TransformParameters transParam, float* xarrScreen, float* yarrScreen) {
	for (uint32_t i = 0; i < n; ++i)
	{
		xarrScreen[i] = (xarr[i] + transParam.offsetX)*transParam.scaleX;
		yarrScreen[i] = (yarr[i] + transParam.offsetY)*transParam.scaleY;
	}
}

void _draw_main(float* xarr = nullptr, float* yarr = nullptr, uint32_t n = 100, float(*func)(float) = nullptr,
	uint32_t left = 0, uint32_t top = 0, uint32_t width = 1080, uint32_t heidth = 400,
	float start = -10, float end = 10) {

	//const uint32_t windowWidth=1080, windowHeidth=400;
	uint32_t i;
	float *xarrScreen, *yarrScreen;

	if (xarr == nullptr) xarr = linspace(start, end, n);

	if (yarr == nullptr) yarr = calculate_values_of_fuction(xarr, n, func);

	TransformParameters transParam =
		calculate_draw_parameters(xarr, yarr, n, left, top, width, heidth);

	xarrScreen = new float[n];
	yarrScreen = new float[n];

	transform_to_screen_coords(xarr, yarr, n, transParam, xarrScreen, yarrScreen);

	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 200));
	SelectObject(hDC, Pen);


	MoveToEx(hDC, xarrScreen[0], yarrScreen[0], NULL);
	for (i = 1; i < n; ++i)
	{
		LineTo(hDC, xarrScreen[i], yarrScreen[i]);
	}

	delete[] xarrScreen; delete[] yarrScreen;
	xarrScreen = nullptr; yarrScreen = nullptr;
}

void draw_schedue(float* xarr, float* yarr, uint32_t n) {
	_draw_main(xarr, yarr, n);
}

void draw_schedue(float* xarr, float* yarr, uint32_t n,
	uint32_t left, uint32_t top, uint32_t width = 1080, uint32_t heidth = 400) {
	_draw_main(xarr, yarr, n, nullptr, left, top, width, heidth);
}

void draw_schedue(float* xarr, uint32_t n, float(*func)(float)) {
	_draw_main(xarr, nullptr, n, func);
}

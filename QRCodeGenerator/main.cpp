/* 
 * QR Code generator demo (C++)
 * 
 * Run this command-line program with no arguments. The program computes a bunch of demonstration
 * QR Codes and prints them to the console. Also, the SVG code for one QR Code is printed as a sample.
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <string>

#include <windows.h>

#include "qrcodegen.hpp"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;


// Function prototypes
static void doBasicDemo(const std::string& wifi);
static std::string toSvgString(const QrCode &qr, int border);
static void printQr(const QrCode &qr);


// The main application program.
int main() {

	const char* ini_file = ".\\wifi_setting.ini";
	char ssid[256] = {};
	if (0 == ::GetPrivateProfileString("Setting", "SSID", "", ssid, sizeof(ssid), ini_file)) {
		std::wcout << L"ini???????????????????????????????????????????????????" << std::endl;
	}
	else {

		// ???????????????????????????
		std::cout << "SSID;" << ssid << std::endl;
	}

	char key[256] = {};
	if (0 == ::GetPrivateProfileString("Setting", "Key", "", key, sizeof(key), ini_file)) {
		std::wcout << L"ini???????????????????????????????????????????????????" << std::endl;
	}
	else {

		// ???????????????????????????
		std::cout << "key;" << key << std::endl;
	}

	std::string wifi = std::string("WIFI:T:WPA;S:") + ssid + std::string(";P:") + key + std::string(";;");

	doBasicDemo(wifi);
	return EXIT_SUCCESS;
}


/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(const std::string& wifi) {
	const char* text = wifi.c_str();

	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level
	
	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text, errCorLvl);
	printQr(qr);
	//std::cout << toSvgString(qr, 4) << std::endl;

	std::ofstream writing_file;
	std::string filename = "sample.svg";
	writing_file.open(filename, std::ios::out);
	std::string writing_text = toSvgString(qr, 4);
	writing_file << writing_text << std::endl;
	writing_file.close();
}


/*---- Utilities ----*/

// Returns a string of SVG code for an image depicting the given QR Code, with the given number
// of border modules. The string always uses Unix newlines (\n), regardless of the platform.
static std::string toSvgString(const QrCode &qr, int border) {
	if (border < 0)
		throw std::domain_error("Border must be non-negative");
	if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
		throw std::overflow_error("Border too large");
	
	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	sb << "\t<path d=\"";
	for (int y = 0; y < qr.getSize(); y++) {
		for (int x = 0; x < qr.getSize(); x++) {
			if (qr.getModule(x, y)) {
				if (x != 0 || y != 0)
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";
	return sb.str();
}


// Prints the given QrCode object to the console.
static void printQr(const QrCode &qr) {
	int border = 4;
	for (int y = -border; y < qr.getSize() + border; y++) {
		for (int x = -border; x < qr.getSize() + border; x++) {
			std::cout << (qr.getModule(x, y) ? "##" : "  ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//input.h header

#ifndef INPUT_H
#define INPUT_H
#include <SFML/Graphics.hpp>
using namespace sf;

class Input
{
	struct Button { bool left, right, z, s, space, escape; };

public:

	Input();
	Button GetButton(void) const;
	void InputHandler(Event event, RenderWindow& window);

private:

	Button button;
};

#endif

//main.h header

#include <iostream>
#include <SFML/Graphics.hpp>
#include "input.h"

const int WIN_WIDTH = 1600;
const int WIN_HEIGHT = 1200;

using namespace sf;
using namespace std;

void SetText(Text& txt, String str);
void CheckBtn();

//input.cpp

#include "input.h"

Input::Input()
{
	button.left = button.right = button.z = button.s = button.space = button.escape = false;
}

Input::Button Input::GetButton(void) const
{
	return button;
}

void Input::InputHandler(Event event, RenderWindow& window)
{
	if (event.type == Event::Closed)
	{
		window.close();
	}

	if (event.type == Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case Keyboard::Escape:
			button.escape = true;
			break;
		case Keyboard::Left:
			button.left = true;
			break;
		case Keyboard::Right:
			button.right = true;
			break;
		case Keyboard::S:
			button.s = true;
			break;
		case Keyboard::Z:
			button.z = true;
			break;
		case Keyboard::Space:
			button.space = true;
			break;
		default:
			break;
		}
	}

	if (event.type == Event::KeyReleased)
	{
		switch (event.key.code)
		{
		case Keyboard::Escape:
			button.escape = false;
			break;
		case Keyboard::Left:
			button.left = false;
			break;
		case Keyboard::Right:
			button.right = false;
			break;
		case Keyboard::S:
			button.s = false;
			break;
		case Keyboard::Z:
			button.z = false;
			break;
		case Keyboard::Space:
			button.space = false;
			break;
		default:
			break;
		}
	}
}

//main.cpp

#include "main.h"

RenderWindow window;
Input input;
Font font;
Text txt;

float posRaquettesLeftX = 50;
float posRaquettesRightX = WIN_WIDTH - 70;
float posRaquettesLeftY = (WIN_HEIGHT / 2) - 100;
float posRaquettesRightY = posRaquettesLeftY;
float raquettesSpeed = 15.f;
int raquettesHeight = 300;
int raquettesWidth = 40;

float ballSpeed = 5.f;
Vector2f ballDir = Vector2f(1.5f, 2.0f);
float ballPosX = WIN_WIDTH / 2;
float ballPosY = (WIN_HEIGHT / 2) - 10;

int scoreJ1 = 0;
int scoreJ2 = 0;

void RaquetteIA()
{
	posRaquettesLeftY = ballPosY - (raquettesHeight/2) + 50;
}

void UpdateBall()
{
	ballPosX += ballDir.x * ballSpeed;
	ballPosY += ballDir.y * ballSpeed;

	if ((ballPosX < posRaquettesLeftX + raquettesWidth &&
		ballPosX > posRaquettesLeftX &&
		ballPosY + 30 < posRaquettesLeftY + raquettesHeight &&
		ballPosY + 30 > posRaquettesLeftY)
		||
		(ballPosX + 25 > posRaquettesRightX - raquettesWidth &&
			ballPosX + 25 < posRaquettesRightX &&
			ballPosY < posRaquettesRightY + raquettesHeight &&
			ballPosY + 30 > posRaquettesRightY))
	{
		ballDir.x *= -1;
		ballSpeed += 0.5f;
	}

	if (ballPosX < 0)
	{
		scoreJ2++;
		ballSpeed = 5.f;
		ballPosX = WIN_WIDTH / 2;
		ballPosY = (WIN_HEIGHT / 2) - 10;
		ballDir.x = fabs(ballDir.x);
		ballDir.y *= -1;
		SetText(txt, to_string(scoreJ1) + "|" + to_string(scoreJ2));
	}

	if (ballPosX > WIN_WIDTH)
	{
		scoreJ1++;
		ballSpeed = 5.f;
		ballPosX = WIN_WIDTH / 2;
		ballPosY = (WIN_HEIGHT / 2) - 10;
		ballDir.x = -fabs(ballDir.x);
		ballDir.y *= -1;
		SetText(txt, to_string(scoreJ1) + "|" + to_string(scoreJ2));
	}

	if (ballPosY > WIN_HEIGHT || ballPosY < 0)
	{
		ballDir.y *= -1;
	}

}


int main()
{
	window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Pong SFML C++", Style::Default);
	window.setVerticalSyncEnabled(true);
	font.loadFromFile("res/poppins.ttf");
	SetText(txt, to_string(scoreJ1) + "|" + to_string(scoreJ2));

	CircleShape circleShape(30);
	circleShape.setPosition(ballPosX, ballPosY);

	RectangleShape rectangleShape(Vector2f(raquettesWidth, raquettesHeight));
	rectangleShape.setPosition(posRaquettesLeftX, posRaquettesLeftY);
	rectangleShape.setOutlineThickness(2);
	rectangleShape.setOutlineColor(Color::Blue);

	RectangleShape rectangleShape2(Vector2f(raquettesWidth, raquettesHeight));
	rectangleShape2.setPosition(posRaquettesRightX, posRaquettesRightY);
	rectangleShape2.setOutlineThickness(2);
	rectangleShape2.setOutlineColor(Color::Red);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			input.InputHandler(event, window);
		}

		CheckBtn();


		if (input.GetButton().space == true)
		{
			RaquetteIA();
		}
	

		rectangleShape.setPosition(posRaquettesLeftX, posRaquettesLeftY);
		rectangleShape2.setPosition(posRaquettesRightX, posRaquettesRightY);

		UpdateBall();
		circleShape.setPosition(ballPosX, ballPosY);

		window.clear(Color::Black);

		window.draw(txt);
		window.draw(circleShape);
		window.draw(rectangleShape);
		window.draw(rectangleShape2);

		window.display();
	}

	return 0;
}

void SetText(Text& txt, String str)
{
	txt.setFont(font);
	txt.setString(str);
	txt.setCharacterSize(52);
	txt.setFillColor(Color::White);
	txt.setPosition((WIN_WIDTH / 2) - 25, 10);
}

void CheckBtn()
{
	if (input.GetButton().z == true)
	{
		posRaquettesLeftY -= raquettesSpeed;
		if (posRaquettesLeftY < 0)
			posRaquettesLeftY = 0;
	}
	if (input.GetButton().s == true)
	{
		posRaquettesLeftY += raquettesSpeed;
		if (posRaquettesLeftY + raquettesHeight > WIN_HEIGHT)
			posRaquettesLeftY = WIN_HEIGHT - raquettesHeight;
	}

	if (input.GetButton().left == true)
	{
		posRaquettesRightY -= raquettesSpeed;
		if (posRaquettesRightY < 0)
			posRaquettesRightY = 0;
	}
	if (input.GetButton().right == true)
	{
		posRaquettesRightY += raquettesSpeed;
		if (posRaquettesRightY + raquettesHeight > WIN_HEIGHT)
			posRaquettesRightY = WIN_HEIGHT - raquettesHeight;
	}

	if (input.GetButton().escape == true)
	{
		window.close();
	}
}


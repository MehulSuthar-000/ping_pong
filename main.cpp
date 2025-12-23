#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "renderer.h"
#include "GameObject.h"

int scorePlayer1{ 0 };
int scorePlayer2{ 0 };

float paddleSpeed{ 1.5f };

float deltaTime{ 0.0f };
float lastFrame{ 0.0f };

const float BALL_SPEED_INCREASE{ 1.1f };
const float BALL_MAX_SPEED{ 5.0f };

int fpsFrames = 0;
float fpsTimer = 0.0f;

GameObject paddle1({ 0.0f, -0.9f }, { 0.3f, 0.05f });
GameObject paddle2({ 0.0f,  0.9f }, { 0.3f, 0.05f });
GameObject ball({ 0.0f,  0.0f }, { 0.05f, 0.05f }, {0.1f, 0.3f});

static void increaseBallSpeed()
{
	float speed = glm::length(ball.velocity);

	if (speed < BALL_MAX_SPEED)
	{
		speed *= BALL_SPEED_INCREASE;
		ball.velocity = glm::normalize(ball.velocity) * speed;
	}
}

static void resetRound(bool player1Scored)
{
	// Reset ball position
	ball.position = { 0.0f, 0.0f };

	// reset paddles
	paddle1.position.x = 0.0f;
	paddle2.position.x = 0.0f;

	// reset ball speed and 7 direction
	float speed = 0.35f;
	ball.velocity.x = (rand() % 2 == 0 ? -1.0f : 1.0f) * 0.2f;
	ball.velocity.y = player1Scored ? -speed : speed;
}

static bool checkCollision(const GameObject& a, const GameObject& b)
{
	bool collisionX{
		std::abs(a.position.x - b.position.x) <= 
		(a.size.x * 0.5f + b.size.x * 0.5f)
	};

	bool collisionY{
		std::abs(a.position.y - b.position.y) <=
		(a.size.y * 0.5 + b.size.y * 0.5f)
	};

	return collisionX && collisionY;
}

static void processInput(GLFWwindow* window)
{
	// Bottom paddle (LEFT / RIGHT)
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		paddle1.position.x -= paddleSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		paddle1.position.x += paddleSpeed * deltaTime;

	// Top paddle (A / D)
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		paddle2.position.x -= paddleSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		paddle2.position.x += paddleSpeed * deltaTime;
	

	float halfWidth1 = paddle1.size.x * 0.5f;
	float halfWidth2 = paddle2.size.x * 0.5f;

	paddle1.position.x = glm::clamp(paddle1.position.x, -1.0f + halfWidth1, 1.0f - halfWidth1);
	paddle2.position.x = glm::clamp(paddle2.position.x, -1.0f + halfWidth2, 1.0f - halfWidth2);

}

int main() {

	// GLFw Initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window{ glfwCreateWindow(1920, 1080, "Ping Pong", NULL, NULL) };
	if (window == NULL)
	{
		std::cout << "Failed to create GLFw window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// GLAD Setup
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	Renderer renderer;

	/// Create shader program

	Shader shader{ "C:\\Users\\mehul\\source\\repos\\ping_pong\\temp.vert",
		"C:\\Users\\mehul\\source\\repos\\ping_pong\\temp.frag" };

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime{ static_cast<float>(glfwGetTime()) };
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		fpsFrames++;
		fpsTimer += deltaTime;

		if (fpsTimer >= 1.0f)
		{
			int fps = fpsFrames;

			float ms = 1000.0f / fps;
			std::string title = "Ping Pong | FPS: " + std::to_string(fps) +
				" | MS: " + std::to_string(ms);
			glfwSetWindowTitle(window, title.c_str());

			fpsFrames = 0;
			fpsTimer = 0.0f;
		}


		processInput(window);

		ball.position += ball.velocity * deltaTime;

		// Paddle collisions
		if (checkCollision(ball, paddle1) && ball.velocity.y < 0.0f)
		{
			float offset{ (ball.position.x - paddle1.position.x) / (paddle1.size.x * 0.5f) };
			offset = glm::clamp(offset, -1.0f, 1.0f);

			float speed{ glm::length(ball.velocity) };

			ball.position.y = paddle1.position.y + paddle1.size.y * 0.5f + ball.size.y * 0.5f;
			ball.velocity.x = offset * speed;
			ball.velocity.y = std::abs(ball.velocity.y);
			ball.velocity = glm::normalize(ball.velocity) * speed;
			increaseBallSpeed();
		}

		if (checkCollision(ball, paddle2) && ball.velocity.y > 0.0f)
		{
			float offset{ (ball.position.x - paddle2.position.x) / (paddle2.size.x * 0.5f) };
			offset = glm::clamp(offset, -1.0f, 1.0f);

			float speed{ glm::length(ball.velocity) };

			ball.position.y = paddle2.position.y - paddle2.size.y * 0.5f - ball.size.y * 0.5f;
			ball.velocity.x = offset * speed;
			ball.velocity.y = -std::abs(ball.velocity.y);
			ball.velocity = glm::normalize(ball.velocity) * speed;
			increaseBallSpeed();
		}


		float ballHalfWidth = ball.size.x * 0.5f;

		if (ball.position.x <= -1.0f + ballHalfWidth ||
			ball.position.x >= 1.0f - ballHalfWidth)
		{
			ball.velocity.x *= -1.0f;
		}

		float ballHalfHeight = ball.size.y * 0.5f;

		// Bottom missed → Player 2 scores
		if (ball.position.y < -1.0f - ballHalfHeight)
		{
			scorePlayer2++;
			std::cout << "Player 2: " << scorePlayer2 << std::endl;
			resetRound(false);
		}

		// Top missed → Player 1 scores
		if (ball.position.y > 1.0f + ballHalfHeight)
		{
			scorePlayer1++;
			std::cout << "Player 1: " << scorePlayer1 << std::endl;
			resetRound(true);
		}


		// frame rendered
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		renderer.draw(shader, paddle1.getModelMatrix());
		renderer.draw(shader, paddle2.getModelMatrix());
		renderer.draw(shader, ball.getModelMatrix());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
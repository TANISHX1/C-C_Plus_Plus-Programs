#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <math.h>

// Vertex Shader: Passes position directly
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader: Applies a time-based color pulsation for a "high-end" dynamic look
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform float time;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(sin(time) * 0.5f + 0.5f, cos(time) * 0.5f + 0.5f, sin(time + 1.0f) * 0.5f + 0.5f, 1.0f);\n"
"}\n\0";

int main()
    {
    // Force X11 backend for GLEW compatibility on Linux/Wayland
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    // Initialize GLFW
    if (!glfwInit())
        {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
        }

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "High-End C++ Graphics Engine", NULL, NULL);
    if (window == NULL)
        {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
        }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
        }

    // Disable V-Sync to unlock max FPS
    glfwSwapInterval(0);

    // Build and compile shader program
    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link Shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers
    float vertices[] = {
        // positions         
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
        };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
        };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO, then bind and set VBO and EBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // Render loop
    while (!glfwWindowShouldClose(window))
        {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render pass
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Update uniform color over time
        float timeValue = glfwGetTime();
        int timeLocation = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(timeLocation, timeValue);

        // Draw rectangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Calculate and update FPS in window title
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
            {
            std::string title = "High-End C++ Graphics Engine - " + std::to_string(nbFrames) + " FPS";
            glfwSetWindowTitle(window, title.c_str());
            nbFrames = 0;
            lastTime += 1.0;
            }
        }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
    }

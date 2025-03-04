#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/transform3D.h"

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}



Mesh* CreatePlane(const std::string& name, glm::vec3 corner, unsigned int m, unsigned int n, glm::vec3 color, bool fill)
{

    std::vector<VertexFormat> vertices;

    for (int i = 0; i < m + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            vertices.push_back(VertexFormat(corner + glm::vec3(i/PLANE_DENSITY, 0, j/PLANE_DENSITY), color));
            //printf("[%d %d]", i, j);
        }
        //printf("\n");
    }


    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices;

    //printf("\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            indices.push_back(i * (n + 1)+j);
            indices.push_back((i + 1) * (n + 1)+j + 1);
            indices.push_back(i * (n + 1) + j + 1);
            //printf("[%d %d %d]", i * (n + 1) + j, (i + 1) * (n + 1) + j + 1, i * (n + 1)+j + 1);
            indices.push_back(i * (n + 1) + j);
            indices.push_back((i + 1) * (n + 1) + j);
            indices.push_back((i + 1) * (n + 1) + j + 1);
        }
        //printf("\n");
    }

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* CreateCylinder(const std::string& name, glm::vec3 baseCenter, float radius, float height, unsigned int sectors, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angleStep = 2.0f * M_PI / sectors;

    for (unsigned int i = 0; i <= sectors; i++) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(VertexFormat(baseCenter + glm::vec3(x, 0, z), color));
        vertices.push_back(VertexFormat(baseCenter + glm::vec3(x, height, z), color));
    }

    for (unsigned int i = 0; i < sectors; i++) {
        unsigned int bottomStart = i * 2;
        unsigned int topStart = bottomStart + 1;

        indices.push_back(bottomStart);
        indices.push_back(topStart);
        indices.push_back(bottomStart + 2);

        indices.push_back(topStart);
        indices.push_back(topStart + 2);
        indices.push_back(bottomStart + 2);
    }

    Mesh* cylinder = new Mesh(name);

    if (!fill) {
        cylinder->SetDrawMode(GL_LINE_LOOP);
    }

    cylinder->InitFromData(vertices, indices);
    return cylinder;
}


Mesh* CreateCone(const std::string& name, glm::vec3 baseCenter, float radius, float height, unsigned int sectors, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angleStep = 2.0f * M_PI / sectors;

    for (unsigned int i = 0; i <= sectors; i++) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(VertexFormat(baseCenter + glm::vec3(x, 0, z), color));
    }

    vertices.push_back(VertexFormat(baseCenter, color));
    unsigned int baseCenterIndex = vertices.size() - 1;

    glm::vec3 tip = baseCenter + glm::vec3(0, height, 0);
    vertices.push_back(VertexFormat(tip, color));
    unsigned int tipIndex = vertices.size() - 1;

    for (unsigned int i = 0; i < sectors; i++) {
        indices.push_back(baseCenterIndex);
        indices.push_back(i);
        indices.push_back((i + 1) % sectors);
    }

    for (unsigned int i = 0; i < sectors; i++) {
        indices.push_back(i);
        indices.push_back((i + 1) % sectors);
        indices.push_back(tipIndex);
    }

    Mesh* cone = new Mesh(name);

    if (!fill) {
        cone->SetDrawMode(GL_LINE_LOOP);
    }

    cone->InitFromData(vertices, indices);
    return cone;
}

Mesh* CreateRectangle(const std::string& name, glm::vec3 center, float width, float height, float depth, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Half dimensions to calculate vertices from the center
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;

    // Define the 8 corners of the 3D rectangle (cuboid) relative to the center
    vertices.push_back(VertexFormat(center + glm::vec3(-halfWidth, -halfHeight, -halfDepth), color)); // 0: Bottom-left-front
    vertices.push_back(VertexFormat(center + glm::vec3(halfWidth, -halfHeight, -halfDepth), color));  // 1: Bottom-right-front
    vertices.push_back(VertexFormat(center + glm::vec3(halfWidth, halfHeight, -halfDepth), color));   // 2: Top-right-front
    vertices.push_back(VertexFormat(center + glm::vec3(-halfWidth, halfHeight, -halfDepth), color));  // 3: Top-left-front

    vertices.push_back(VertexFormat(center + glm::vec3(-halfWidth, -halfHeight, halfDepth), color));  // 4: Bottom-left-back
    vertices.push_back(VertexFormat(center + glm::vec3(halfWidth, -halfHeight, halfDepth), color));   // 5: Bottom-right-back
    vertices.push_back(VertexFormat(center + glm::vec3(halfWidth, halfHeight, halfDepth), color));    // 6: Top-right-back
    vertices.push_back(VertexFormat(center + glm::vec3(-halfWidth, halfHeight, halfDepth), color));   // 7: Top-left-back

    // Define the indices for the 12 triangles (2 per face)
    // Front face
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);

    // Back face
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(4); indices.push_back(6); indices.push_back(7);

    // Left face
    indices.push_back(0); indices.push_back(3); indices.push_back(7);
    indices.push_back(0); indices.push_back(7); indices.push_back(4);

    // Right face
    indices.push_back(1); indices.push_back(2); indices.push_back(6);
    indices.push_back(1); indices.push_back(6); indices.push_back(5);

    // Top face
    indices.push_back(3); indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(7);

    // Bottom face
    indices.push_back(0); indices.push_back(1); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(4);

    // Create the mesh
    Mesh* rectangle3D = new Mesh(name);

    if (!fill) {
        rectangle3D->SetDrawMode(GL_LINE_LOOP);
    }

    rectangle3D->InitFromData(vertices, indices);
    return rectangle3D;
}

Mesh* CreateTrianglePrism(const std::string& name, glm::vec3 center, float baseWidth, float height, float depth, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float halfWidth = baseWidth / 2.0f;
    float halfDepth = depth / 2.0f;
    float halfHeight = height / 2.0f;

    glm::vec3 v0 = center + glm::vec3(-halfWidth, -halfHeight, -halfDepth);
    glm::vec3 v1 = center + glm::vec3(halfWidth, -halfHeight, -halfDepth);
    glm::vec3 v2 = center + glm::vec3(0, -halfHeight, halfDepth);
    glm::vec3 v3 = center + glm::vec3(-halfWidth, halfHeight, -halfDepth);
    glm::vec3 v4 = center + glm::vec3(halfWidth, halfHeight, -halfDepth);
    glm::vec3 v5 = center + glm::vec3(0, halfHeight, halfDepth);

    vertices.push_back(VertexFormat(v0, color));
    vertices.push_back(VertexFormat(v1, color));
    vertices.push_back(VertexFormat(v2, color));
    vertices.push_back(VertexFormat(v3, color));
    vertices.push_back(VertexFormat(v4, color));
    vertices.push_back(VertexFormat(v5, color));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(3);
    indices.push_back(5);
    indices.push_back(4);

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(1);

    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(5);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    Mesh* trianglePrism = new Mesh(name);

    if (!fill) {
        trianglePrism->SetDrawMode(GL_LINE_LOOP);
    }

    trianglePrism->InitFromData(vertices, indices);
    return trianglePrism;
}

Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, unsigned int sectors, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angleStep = 2.0f * M_PI / sectors;

    vertices.push_back(VertexFormat(center, color));

    for (unsigned int i = 0; i <= sectors; i++) {
        float angle = i * angleStep;
        float x = center.x + radius * cos(angle);
        float z = center.z + radius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, center.y, z), color));
    }

    for (unsigned int i = 1; i <= sectors; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int is_plane)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_object_is_plane = glGetUniformLocation(shader->program, "object_is_plane");
    glUniform1i(loc_object_is_plane, is_plane);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    GLint loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    GLint loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::Init()
{
    polygonMode = GL_FILL;

    
    {
        Mesh* square = CreatePlane("plane", glm::vec3(0, 0, 0), PLANE_SIZE * PLANE_DENSITY, PLANE_SIZE * PLANE_DENSITY, glm::vec3(1, 0, 0), true);
        AddMeshToList(square);

        Mesh* cylinder = CreateCylinder("cylinder", glm::vec3(0, 0, 0), TRUNK_RADIUS, TRUNK_HEIGHT, 30, glm::vec3(0.396f, 0.196f, 0.051f), true);
        AddMeshToList(cylinder);

        Mesh* cone = CreateCone("cone", glm::vec3(0, 0, 0), CONE_RADIUS, CONE_HEIGHT, 30, glm::vec3(0, 0.32, 0), true);
        AddMeshToList(cone);

        Mesh* droneWing = CreateRectangle("droneWing", glm::vec3(0, 0, 0), DRONE_WING_LENGTH, DRONE_WING_WIDTH, DRONE_WING_WIDTH, glm::vec3(0.5, 0.5, 0.5), true);
        AddMeshToList(droneWing);

        Mesh* droneCube = CreateRectangle("droneCube", glm::vec3(0, 0, 0), DRONE_WING_WIDTH, DRONE_WING_WIDTH, DRONE_WING_WIDTH, glm::vec3(0.5, 0.5, 0.5), true);
        AddMeshToList(droneCube);

        Mesh* dronePropeller = CreateRectangle("dronePropeller", glm::vec3(0, 0, 0), PROPELLER_WING_LENGTH, PROPELLER_WING_WIDTH, PROPELLER_WING_WIDTH, glm::vec3(0, 0, 0), true);
        AddMeshToList(dronePropeller);

        Mesh* building = CreateRectangle("building", glm::vec3(0, 0, 0), BUILDING_LENTGH, BUILDING_HEIGHT, BUILDING_WIDTH, glm::vec3(0, 0, 0), true);
        AddMeshToList(building);

        Mesh* package = CreateRectangle("package", glm::vec3(0, 0, 0), PACKAGE_LENTGH, PACKAGE_HEIGHT, PACKAGE_WIDTH, glm::vec3(0, 0, 0.6), true);
        AddMeshToList(package);

        Mesh* checkpoint = CreateCylinder("checkpoint", glm::vec3(0, 0, 0), CHECKPOINT_RADIUS, CHECKPOINT_HEIGHT, 30, glm::vec3(1, 0, 0), true);
        AddMeshToList(checkpoint);

        Mesh* arrowBase = CreateRectangle("arrowBase", glm::vec3(0, 0, 0), ARROW_BASE_LENGTH, ARROW_BASE_HEIGHT, ARROW_BASE_WIDTH, glm::vec3(242.0f / 255.0f, 183.0f / 255.0f, 0), true);
        AddMeshToList(arrowBase);

        Mesh* arrowTriangle = CreateTrianglePrism("arrowTriangle", glm::vec3(0, 0, 0), ARROW_TRIANGLE_BASE_WIDTH, ARROW_TRIANGLE_HEIGHT, ARROW_TRIANGLE_LENGTH, glm::vec3(242.0f / 255.0f, 183.0f / 255.0f, 0), true);
        AddMeshToList(arrowTriangle);

        Mesh* drone_indicator = CreateCircle("drone_indicator", glm::vec3(0, 0, 0), MINIMAP_INDICATOR_RADIUS, 30, glm::vec3(1, 1, 1), true);
        AddMeshToList(drone_indicator);

        Mesh* checkpoint_indicator = CreateCircle("checkpoint_indicator", glm::vec3(0, 0, 0), MINIMAP_INDICATOR_RADIUS, 30, glm::vec3(1, 0, 0), true);
        AddMeshToList(checkpoint_indicator);

        Mesh* package_indicator = CreateCircle("package_indicator", glm::vec3(0, 0, 0), MINIMAP_INDICATOR_RADIUS, 30, glm::vec3(0, 0, 1), true);
        AddMeshToList(package_indicator);

        Mesh* cloud = CreateRectangle("cloud", glm::vec3(0, 0, 0), CLOUD_SCALE, CLOUD_THICKNESS, CLOUD_SCALE, glm::vec3(1, 1, 1), true);
        AddMeshToList(cloud);

        Mesh* mesh = new Mesh("1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "1.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "2.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("3");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "3.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("4");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "4.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("5");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "5.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("6");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "6.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("7");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "7.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("8");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "8.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("9");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "9.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("0");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "0.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("A");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "A.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("B");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "B.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("C");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "C.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("D");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "D.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("E");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "E.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("F");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "F.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("G");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "G.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("H");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "H.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("I");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "I.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("J");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "J.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("K");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "K.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("L");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "L.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("M");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "M.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("N");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "N.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("O");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "O.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("P");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "P.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("Q");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Q.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("R");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "R.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("S");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "S.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("T");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "T.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("U");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "U.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("V");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "V.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("W");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "W.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("X");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "X.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("Y");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Y.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("Z");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Z.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("enemy");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "FinalBaseMesh.obj");
        meshes[mesh->GetMeshID()] = mesh;

        Mesh* bullet = CreateRectangle("bullet", glm::vec3(0, 0, 0), BULLET_SIZE, BULLET_SIZE, BULLET_SIZE, glm::vec3(0.7, 0.7, 0), true);
        AddMeshToList(bullet);
    }


    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f); 

    Shader* shader = new Shader("MyShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;


    //generate trees
    for (int i = 0; i < NUM_TREES; i++) {
        Tree tree;
        tree.pos = glm::vec3(rand()/1.0f /RAND_MAX * PLANE_SIZE - PLANE_SIZE/2, 0, rand() / 1.0f / RAND_MAX * PLANE_SIZE - PLANE_SIZE/2);
        //chesk if trees are hitting each other
        bool ok = true;
        for (auto& t : trees) {
            if (t.isHittingTree(tree.pos) || (tree.pos.x * tree.pos.x + tree.pos.z * tree.pos.z <= (CHECKPOINT_RADIUS + 5.0f) * (CHECKPOINT_RADIUS + 5.0f))) {
                ok = false;
                break;
            }
        }
        if (ok) {
            trees.push_back(tree);
            
        }
    }

    //generate clouds
    for (int i = -PLANE_SIZE/2; i < PLANE_SIZE/2; i+= CLOUD_SCALE) {
        for (int j = -PLANE_SIZE/2; j < PLANE_SIZE/2; j += CLOUD_SCALE) {
            if (1.0f * rand() / RAND_MAX < CLOUD_DENSITY) {
                Cloud c;
                c.pos = glm::vec3(i, CLOUD_HIGHT_FROM_GROUND, j);
                clouds.push_back(c);
            }
        }
    }

    //generate enemies
    for (int i = 0; i < NUM_ENEMIES; i++) {
        bool ok = true;
        Enemy enemy;
        enemy.pos = glm::vec3(rand() / 1.0f / RAND_MAX * PLANE_SIZE - PLANE_SIZE / 2, 0, rand() / 1.0f / RAND_MAX * PLANE_SIZE - PLANE_SIZE / 2);
        for (auto& t : trees) {
            if (t.isHittingTree(enemy.pos) || (enemy.pos.x * enemy.pos.x + enemy.pos.z * enemy.pos.z <= (CHECKPOINT_RADIUS + 5.0f) * (CHECKPOINT_RADIUS + 5.0f))) {
                ok = false;
                break;
            }
        }
        if (ok) {
            enemies.push_back(enemy);

        }
    }

    //init drones
    drone1.pos = glm::vec3(0, DRONE_INIT_HIGHT, 0);
    drones.push_back(drone1);

    //print keybinds
    system("cls");
    printf("~~~~~~~~~~CONTROLS~~~~~~~~~~\n");
    printf("W/A/S/D - MOVE DRONE\n");\
    printf("Q/E - ROTATE DRONE\n");
    printf("SHIFT/SPACE - UP/DOWN DRONE\n");
    printf("\nX - THIRD PERSON/FIRST PERSON\n");
    printf("B - RESET SCORE AND TIMER\n");
    printf("SCROLL WHEEL - CHANGE DRONE SPEED\n");
    printf("\n\n~~~~~~~~~~CHEATS~~~~~~~~~~\n");
    printf("M - REGEN PACKAGE LOCATION\n");
    printf("N - TAKE PACKAGE WIRELESSLY =))\n");




}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


float FindRotationOY(glm::vec3 pos1, glm::vec3 pos2) {
    return atan2(pos2.x - pos1.x, pos2.z - pos1.z); 
}

void Tema2::RenderScene(float deltaTimeSeconds, int flag) {

    glm::mat4 modelMatrix = glm::mat4(1);

    //camera
    if (flag == 0) {
        if (thirdPerson){
            glm::vec3 cameraPos = drone1.pos - glm::vec3(sin(drone1.rotation) * CAMERA_DISTANCE, -CAMERA_HEIGHT, cos(drone1.rotation) * CAMERA_DISTANCE);
            glm::vec3 cameraRot = glm::vec3(-CAMERA_ANGLE, drone1.rotation + RADIANS(180), 0);
            GetSceneCamera()->SetPositionAndRotation(cameraPos, cameraRot);
            modelMatrix = glm::mat4(1) = GetSceneCamera()->GetViewMatrix();
        }
        else {
            glm::vec3 cameraPos = drone1.pos + glm::vec3(0, DRONE_WING_WIDTH/2 + 0.17, 0);
            glm::vec3 cameraRot = glm::vec3(RADIANS(-30), drone1.rotation + RADIANS(180), 0);
            GetSceneCamera()->SetPositionAndRotation(cameraPos, cameraRot);
        }
    }
    else if (flag == 1) {
        glm::vec3 cameraPos = glm::vec3(drone1.pos.x, MINIMAP_HEIGHT, drone1.pos.z);
        glm::vec3 cameraRot = glm::vec3(RADIANS(-90), drone1.rotation + RADIANS(180), 0);
        GetSceneCamera()->SetPositionAndRotation(cameraPos, cameraRot);
    }

    //renders plane
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-PLANE_SIZE/2, 0, -PLANE_SIZE/2);
    RenderSimpleMesh(meshes["plane"], shaders["MyShader"], modelMatrix, 1);

    //renders trees
    for (int i = 0; i < trees.size(); i++) {
        glm::vec3 pos = trees[i].pos;
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x , 0, pos.z);
        RenderSimpleMesh(meshes["cylinder"], shaders["VertexColor"], modelMatrix, 0);
        modelMatrix *= transform3D::Translate(0, TRUNK_HEIGHT, 0);
        RenderSimpleMesh(meshes["cone"], shaders["VertexColor"], modelMatrix, 0);
        modelMatrix *= transform3D::Translate(0, TRUNK_HEIGHT/3*2, 0);
        RenderSimpleMesh(meshes["cone"], shaders["MyShader"], modelMatrix, 0);

    }

    //renders drone
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(drone1.pos.x, drone1.pos.y, drone1.pos.z);
    modelMatrix *= transform3D::RotateOY(RADIANS(45) + drone1.rotation);
    RenderSimpleMesh(meshes["droneWing"], shaders["VertexColor"], modelMatrix, 0);
    modelMatrix *= transform3D::RotateOY(RADIANS(90));
    RenderSimpleMesh(meshes["droneWing"], shaders["VertexColor"], modelMatrix, 0);
    modelMatrix *= transform3D::Translate(0, DRONE_WING_WIDTH, 0);
    RenderSimpleMesh(meshes["droneCube"], shaders["VertexColor"], modelMatrix * transform3D::Translate(DRONE_WING_LENGTH / 2 - DRONE_WING_WIDTH / 2, 0, 0), 0);
    RenderSimpleMesh(meshes["droneCube"], shaders["VertexColor"], modelMatrix * transform3D::Translate(0, 0, DRONE_WING_LENGTH / 2 - DRONE_WING_WIDTH / 2), 0);
    RenderSimpleMesh(meshes["droneCube"], shaders["VertexColor"], modelMatrix * transform3D::Translate(-DRONE_WING_LENGTH / 2 + DRONE_WING_WIDTH / 2, 0, 0), 0);
    RenderSimpleMesh(meshes["droneCube"], shaders["VertexColor"], modelMatrix * transform3D::Translate(0, 0, -DRONE_WING_LENGTH / 2 + DRONE_WING_WIDTH / 2), 0);
    modelMatrix *= transform3D::Translate(0, DRONE_WING_WIDTH/2 + PROPELLER_WING_WIDTH/2, 0);
    drone1.propellerSpin += deltaTimeSeconds*20;
    RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], modelMatrix * transform3D::Translate(DRONE_WING_LENGTH / 2 - DRONE_WING_WIDTH / 2, 0, 0) * transform3D::RotateOY(drone1.propellerSpin), 0);
    RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], modelMatrix * transform3D::Translate(0, 0, DRONE_WING_LENGTH / 2 - DRONE_WING_WIDTH / 2) * transform3D::RotateOY(drone1.propellerSpin), 0);
    RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], modelMatrix * transform3D::Translate(-DRONE_WING_LENGTH / 2 + DRONE_WING_WIDTH / 2, 0, 0) * transform3D::RotateOY(drone1.propellerSpin), 0);
    RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], modelMatrix * transform3D::Translate(0, 0, -DRONE_WING_LENGTH / 2 + DRONE_WING_WIDTH / 2) * transform3D::RotateOY(drone1.propellerSpin), 0);

    glm::mat4 modelMatrixDrone = modelMatrix;

    //arrow
    if (thirdPerson) {
        if (!package.hooked) {
            arrow.rotation = FindRotationOY(drone1.pos, package.pos);
        }
        else {
            arrow.rotation = FindRotationOY(drone1.pos, glm::vec3(0, 0, 0));
        }
        modelMatrix *= transform3D::RotateOY(arrow.rotation + RADIANS(-45) - drone1.rotation);
        modelMatrix *= transform3D::Translate(0, ARROW_DIST_ABOVE_DRONE, 0);
        RenderSimpleMesh(meshes["arrowBase"], shaders["VertexColor"], modelMatrix, 0);
        modelMatrix *= transform3D::Translate(-ARROW_BASE_LENGTH / 2 - ARROW_TRIANGLE_LENGTH / 2, 0, 0);
        modelMatrix *= transform3D::RotateOY(RADIANS(-90));
        RenderSimpleMesh(meshes["arrowTriangle"], shaders["VertexColor"], modelMatrix, 0);
    }
    else {
        if (!package.hooked) {
            arrow.rotation = FindRotationOY(drone1.pos, package.pos);
        }
        else {
            arrow.rotation = FindRotationOY(drone1.pos, glm::vec3(0, 0, 0));
        }
        modelMatrix *= transform3D::Translate(-1, 0, -1);
        modelMatrix *= transform3D::RotateOY(arrow.rotation + RADIANS(-45) - drone1.rotation);
        modelMatrix *= transform3D::Translate(0, ARROW_DIST_ABOVE_DRONE, 0);
        RenderSimpleMesh(meshes["arrowBase"], shaders["VertexColor"], modelMatrix, 0);
        modelMatrix *= transform3D::Translate(-ARROW_BASE_LENGTH / 2 - ARROW_TRIANGLE_LENGTH / 2, 0, 0);
        modelMatrix *= transform3D::RotateOY(RADIANS(-90));
        RenderSimpleMesh(meshes["arrowTriangle"], shaders["VertexColor"], modelMatrix, 0);
    }

    //scoreBoard
    modelMatrix = modelMatrixDrone;
    modelMatrix *= transform3D::RotateOY(RADIANS(45));
    modelMatrix *= transform3D::RotateOX(-CAMERA_ANGLE);
    timePassed += deltaTimeSeconds;
    float offset1 = 0.0f;
    float offset2 = 0.0f;
    float offset3 = 0.0f;
    float offset4 = 0.0f;
    if (int(timePassed) - timePassedInt >= 1.0f) {
        timePassedInt++;
        if (timePassedInt % BULLET_SHOOT_DELAY == 0){
            shootEnemy = true;
        }
        line1 = "SCORE " + to_string(points);
        offset1 = 0.0f;

        line2 = "TIME PASSED " + to_string(timePassedInt);
        offset2 = 0.0f;

        line3 = "HEALTH " + to_string(drone1.health);
        offset3 = 0.0f;

        line4 = "ENEMIES LEFT " + to_string(enemies.size());
        offset4 = 0.0f;
    }
    for (const auto c : line1) {
        if (isalpha(c)) {
            RenderSimpleMesh(meshes[string(1, c)], shaders["VertexColor"], modelMatrix * transform3D::Translate(-4.35f + offset1, 2.2, 0) * transform3D::Scale(.0002, .0002, .00001), 0);
        }
        else {
            RenderSimpleMesh(meshes[string(1, c)], shaders["MyShader"], modelMatrix * transform3D::Translate(-4.35f + offset1, 2.12, 0) * transform3D::Scale(.0002, .0002, .00001), 2);
        }
        offset1 += 0.2f;
    }
    for (const auto c : line2) {
        if (isalpha(c)) {
            RenderSimpleMesh(meshes[string(1, c)], shaders["VertexColor"], modelMatrix * transform3D::Translate(-4.35f + offset2, 1.95, 0) * transform3D::Scale(.0002, .0002, .00001), 0);
        }
        else {
            RenderSimpleMesh(meshes[string(1, c)], shaders["MyShader"], modelMatrix * transform3D::Translate(-4.35f + offset2, 1.87, 0) * transform3D::Scale(.0002, .0002, .00001), 2);
        }
        offset2 += 0.2f;
    }
    for (const auto c : line3) {
        if (isalpha(c)) {
            RenderSimpleMesh(meshes[string(1, c)], shaders["VertexColor"], modelMatrix * transform3D::Translate(-4.35f + offset3, 1.7, 0) * transform3D::Scale(.0002, .0002, .00001), 0);
        }
        else {
            if (drone1.health > 50) {
                RenderSimpleMesh(meshes[string(1, c)], shaders["MyShader"], modelMatrix * transform3D::Translate(-4.35f + offset3, 1.62, 0) * transform3D::Scale(.0002, .0002, .00001), 3);
            }
            else {
                RenderSimpleMesh(meshes[string(1, c)], shaders["MyShader"], modelMatrix * transform3D::Translate(-4.35f + offset3, 1.62, 0) * transform3D::Scale(.0002, .0002, .00001), 2);
            }
        }
        offset3 += 0.2f;
    }
    for (const auto c : line4) {
        if (isalpha(c)) {
            RenderSimpleMesh(meshes[string(1, c)], shaders["VertexColor"], modelMatrix * transform3D::Translate(1.0f + offset4, 2.2, 0) * transform3D::Scale(.0002, .0002, .00001), 0);
        }
        else {
            RenderSimpleMesh(meshes[string(1, c)], shaders["MyShader"], modelMatrix * transform3D::Translate(1.0f + offset4, 2.12, 0) * transform3D::Scale(.0002, .0002, .00001), 2);
        }
        offset4 += 0.2f;
    }


    //package
    if ( pow((package.pos.x - drone1.pos.x), 2) + pow((package.pos.y - drone1.pos.y), 2) <= pow(DRONE_WING_LENGTH/2 + PACKAGE_WIDTH/2, 2) &&
        drone1.pos.y <= package.pos.y + PACKAGE_HEIGHT / 2 && drone1.pos.y >= package.pos.y - PACKAGE_HEIGHT / 2) {
        package.hooked = true;
    }

    if (package.delivered) {
        //generate new package location
        package.delivered = false;
        package.hooked = false;
        bool ok = false;
        while (!ok) {
            ok = true;
            package.pos = glm::vec3(rand() / 1.0f / RAND_MAX * PLANE_SIZE - PLANE_SIZE / 2,PACKAGE_HEIGHT / 2, rand() / 1.0f / RAND_MAX * PLANE_SIZE - PLANE_SIZE / 2);
            for (auto& t : trees) {
                if (t.isHittingTree(package.pos + glm::vec3(PACKAGE_WIDTH/2, 0, PACKAGE_WIDTH/2)) ||
                    t.isHittingTree(package.pos + glm::vec3(PACKAGE_WIDTH / 2, 0, -PACKAGE_WIDTH / 2)) ||
                    t.isHittingTree(package.pos + glm::vec3(-PACKAGE_WIDTH / 2, 0, PACKAGE_WIDTH / 2)) ||
                    t.isHittingTree(package.pos + glm::vec3(-PACKAGE_WIDTH / 2, 0, -PACKAGE_WIDTH / 2))) {
                    ok = false;
                    break;
                }
            }
        }
    }  
    if (package.hooked) {
        package.pos = drone1.pos - glm::vec3(0, PACKAGE_DISTANCE_BELOW_DRONE, 0);
    }
    if (package.pos.x * package.pos.x + package.pos.z * package.pos.z <= pow(CHECKPOINT_RADIUS + PACKAGE_WIDTH/2, 2) && package.pos.y - PACKAGE_HEIGHT / 2 <= CHECKPOINT_HEIGHT) {
        package.delivered = true;
        points++;
        printf("Pachet Livrat\n");
    }
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(package.pos.x, package.pos.y, package.pos.z);
    RenderSimpleMesh(meshes["package"], shaders["VertexColor"], modelMatrix, 0);

    //checkpoint
    modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["checkpoint"], shaders["MyShader"], modelMatrix, 5);

    

    //indicators on minimap
    if (flag == 1) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(drone1.pos.x, MINIMAP_INDICATOR_HEIGHT, drone1.pos.z);
        RenderSimpleMesh(meshes["drone_indicator"], shaders["VertexColor"], modelMatrix, 0);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, MINIMAP_INDICATOR_HEIGHT, 0);
        RenderSimpleMesh(meshes["checkpoint_indicator"], shaders["VertexColor"], modelMatrix, 0);

        if (!package.hooked) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(package.pos.x, MINIMAP_INDICATOR_HEIGHT, package.pos.z);
            RenderSimpleMesh(meshes["package_indicator"], shaders["VertexColor"], modelMatrix, 0);
        }
    }

    //clouds
    if (flag != 1){
        modelMatrix = glm::mat4(1);
        for (auto& c : clouds) {
            RenderSimpleMesh(meshes["cloud"], shaders["VertexColor"], modelMatrix * transform3D::Translate(c.pos.x, c.pos.y, c.pos.z), 0);
        }
    }

    if (flag == 0) {
        //enemies
        //removes enemies if dead
        auto it1 = remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
            return e.health <= 0.0f && e.deathAnimation >= RADIANS(90); });
        enemies.erase(it1, enemies.end());

        for (auto& e : enemies) {
            if (e.health <= 0 && e.deathAnimation >= RADIANS(90)) {
                continue;
            }
            e.rotation = FindRotationOY(drone1.pos, e.pos) + RADIANS(180);
            if (shootEnemy && e.health > 0) {
                Bullet bullet;
                bullet.owner = ENEMY;
                bullet.initialPos = e.pos + glm::vec3(0, 8, 0);
                bullet.pos = bullet.initialPos;
                glm::vec3 direction = drone1.pos - bullet.initialPos;
                bullet.direction = glm::normalize(direction);
                float yaw = atan2(direction.x, direction.z);                              
                float pitch = atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)); 
                float roll = 0.0f;
                bullet.rotation = glm::vec3(pitch, yaw, roll);
                bullets.push_back(bullet);
            }
            if (e.health <= 0) {
                e.deathAnimation += ENEMY_DEATH_ANIMATION_SPEED;
            }
            RenderSimpleMesh(meshes["enemy"], shaders["VertexNormal"], modelMatrix  *
                transform3D::Translate(e.pos.x, e.pos.y, e.pos.z) * transform3D::Scale(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE) * transform3D::RotateOY(e.rotation) *
                transform3D::RotateOZ(e.deathAnimation), 0);
        }
        shootEnemy = false;

        //bullets
        //remove bullets if ttl expires
        auto it2 = remove_if(bullets.begin(), bullets.end(), [](const auto& b) {
            return b.timeLived >= BULLET_TTL; });
        bullets.erase(it2, bullets.end());

        for (auto &b : bullets) {
            b.timeLived += deltaTimeSeconds;
            if (b.hitDrone(drone1.pos) && b.owner == ENEMY) {
                b.timeLived = 10; 
                drone1.health -= BULLET_DAMAGE_ENEMY;
            }
            for (auto& e : enemies) {
                if (b.owner == DRONE && b.hitEnemy(e.pos)) {
                    e.health = 0;
                }
            }
            b.pos += b.direction * deltaTimeSeconds * BULLET_SPEED;
            RenderSimpleMesh(meshes["bullet"], shaders["VertexColor"], modelMatrix * transform3D::Translate(b.pos.x, b.pos.y, b.pos.z), 0);
        }
    }

    if (drone1.health <= 0) {
        drone1.pos = glm::vec3(0, DRONE_INIT_HIGHT, 0);
        drone1.rotation = 0;
        drone1.health = 100;
        points = 0;
        timePassedInt = 0;
        timePassed = 0.0f;
    }

}

void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
    RenderScene(deltaTimeSeconds, 0);

    //DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, resolution.x / MINIMAP_SCALE, resolution.y / MINIMAP_SCALE);

    // TODO(student): render the scene again, in the new viewport
    RenderScene(deltaTimeSeconds, 1);
    //DrawCoordinateSystem();
}

void Tema2::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec3 diagonalRightDirection = glm::vec3(sin(drone1.rotation - RADIANS(45)), 0.0f, cos(drone1.rotation - RADIANS(45)));
    glm::vec3 diagonalLeftDirection = glm::vec3(sin(drone1.rotation + RADIANS(45)), 0.0f, cos(drone1.rotation + RADIANS(45)));
    glm::vec3 forwardDirection = glm::vec3(sin(drone1.rotation), 0.0f, cos(drone1.rotation));
    glm::vec3 rightDirection = glm::vec3(cos(drone1.rotation), 0.0f, -sin(drone1.rotation));
    glm::vec3 upDirection = glm::vec3(0, 1.0f, 0);
    if (window->KeyHold(GLFW_KEY_W)) {
        bool ok = true;
        for (auto& tree : trees) {
            if (tree.hitbox(drone1.pos) ||
                tree.hitbox(drone1.pos + diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos + diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            drone1.pos += deltaTime * forwardDirection * drone1.speed;
        }
        else {
            drone1.pos -= deltaTime * forwardDirection * drone1.speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        bool ok = true;
        for (auto& tree : trees) {
            if (tree.hitbox(drone1.pos) ||
                tree.hitbox(drone1.pos - diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos - diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            drone1.pos -= deltaTime * forwardDirection * drone1.speed;
        }
        else {
            drone1.pos += deltaTime * forwardDirection * drone1.speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        bool ok = true;
        for (auto& tree : trees) {
            if (tree.hitbox(drone1.pos) ||
                tree.hitbox(drone1.pos - diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos + diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            drone1.pos += deltaTime * rightDirection * drone1.speed;
        }
        else {
            drone1.pos -= deltaTime * rightDirection * drone1.speed;
        }
        
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        bool ok = true;
        for (auto& tree : trees) {
            if (tree.hitbox(drone1.pos) ||
                tree.hitbox(drone1.pos + diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos - diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            drone1.pos -= deltaTime * rightDirection * drone1.speed;
        }
        else {
            drone1.pos += deltaTime * rightDirection * drone1.speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_SPACE)) {
        bool ok = true;
        for (auto& tree : trees) {
            if (tree.hitbox(drone1.pos + upDirection * DRONE_WING_WIDTH) ||
                tree.hitbox(drone1.pos + diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos - diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos + diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos - diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            drone1.pos += deltaTime * upDirection * drone1.speed;
        }
        else {
            drone1.pos -= deltaTime * upDirection * drone1.speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
        bool ok = true;
        for (auto& tree : trees) {
            if (tree.hitbox(drone1.pos - upDirection * DRONE_WING_WIDTH) ||
                tree.hitbox(drone1.pos + diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos - diagonalRightDirection * DRONE_WING_LENGTH * 0.5f) ||
                tree.hitbox(drone1.pos + diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f) ||
                drone1.pos.y <= 1.0f ||
                tree.hitbox(drone1.pos - diagonalLeftDirection * DRONE_WING_LENGTH * 0.5f)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            drone1.pos -= deltaTime * upDirection * drone1.speed;
        }
        else {
            drone1.pos += deltaTime * upDirection * drone1.speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        drone1.rotation += deltaTime * drone1.rotationSpeed;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        drone1.rotation -= deltaTime * drone1.rotationSpeed;
    }
    
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_RIGHT_CONTROL)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_M) {
        package.delivered = true;
    }
    if (key == GLFW_KEY_N) {
        package.hooked = true;
    }
    if (key == GLFW_KEY_X) {
        thirdPerson = !thirdPerson;
    }
    if (key == GLFW_KEY_B) {
        points = 0;
        timePassed = 0.0f;
        timePassedInt = 0;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{

}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {
        Bullet bullet;
        bullet.owner = DRONE;
        bullet.initialPos = drone1.pos + glm::vec3(0, 0, 0);
        bullet.pos = bullet.initialPos;
        glm::vec3 direction = glm::vec3(sin(drone1.rotation), 0, cos(drone1.rotation));
        bullet.direction = glm::normalize(direction);
        float yaw = atan2(direction.x, direction.z);
        float pitch = atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));
        float roll = 0.0f;
        bullet.rotation = glm::vec3(pitch, yaw, roll);
        bullets.push_back(bullet);
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    drone1.speed += offsetY;
}


void Tema2::OnWindowResize(int width, int height)
{
}

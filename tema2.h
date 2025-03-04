#pragma once

#include "components/simple_scene.h"


//plane
#define PLANE_SIZE (200.0f)
#define PLANE_DENSITY (10.0f)

//trees
#define NUM_TREES (300.0f)
#define TRUNK_HEIGHT (1.5f)
#define TRUNK_RADIUS (0.25f)
#define CONE_RADIUS (1.0f)
#define CONE_HEIGHT (1.75f)

//drone
#define DRONE_WING_LENGTH (0.75f)
#define DRONE_WING_WIDTH (0.0625f)

#define PROPELLER_WING_LENGTH (0.25f)
#define PROPELLER_WING_WIDTH (0.0125)

#define DRONE_SPEED (5.0f)
#define DRONE_ROTATION_SPEED (1.5f)

#define DRONE_INIT_HIGHT (10.0f)

//building
#define BUILDING_HEIGHT (1.0f)
#define BUILDING_WIDTH (1.0f)
#define BUILDING_LENTGH (1.0f)

//package
#define PACKAGE_HEIGHT (1.0f)
#define PACKAGE_WIDTH (1.0f)
#define PACKAGE_LENTGH (1.0f)
#define PACKAGE_DISTANCE_BELOW_DRONE (1.5f)

//camera
#define CAMERA_DISTANCE (3.75f)
#define CAMERA_HEIGHT (2.25f)
#define CAMERA_ANGLE (atan(CAMERA_HEIGHT / CAMERA_DISTANCE))

//checkpoint
#define CHECKPOINT_RADIUS (2.5f)
#define CHECKPOINT_HEIGHT (2.5f)

//arrow
#define ARROW_DIST_ABOVE_DRONE (-1.0f)

#define ARROW_BASE_LENGTH (0.5f)
#define ARROW_BASE_HEIGHT (0.1f)
#define ARROW_BASE_WIDTH (0.2f)

#define ARROW_TRIANGLE_BASE_WIDTH (0.4f)
#define ARROW_TRIANGLE_HEIGHT (0.1f)
#define ARROW_TRIANGLE_LENGTH (0.4f)

//minimap
#define MINIMAP_INDICATOR_RADIUS (6.0f)
#define MINIMAP_INDICATOR_HEIGHT (5.0f)
#define MINIMAP_HEIGHT (200.0f)
#define MINIMAP_SCALE (3.0f)

//clouds
#define CLOUD_SCALE (10.0f)
#define CLOUD_HIGHT_FROM_GROUND (20.0f)
#define CLOUD_THICKNESS (3.0f)
#define CLOUD_DENSITY (0.5f) //between 0 and 1

//enemies
#define NUM_ENEMIES (5.0f)
#define ENEMY_SCALE (0.5f)
#define ENEMY_HEIGHT (20.0f)
#define ENEMY_WIDTH (3.0f)
#define ENEMY_DEATH_ANIMATION_SPEED RADIANS(1)

//bullet
#define BULLET_SPEED (50.0f)
#define BULLET_SHOOT_DELAY 5
#define BULLET_TTL (10.0f)
#define BULLET_SIZE (0.1f)
#define BULLET_DAMAGE_ENEMY (10)
#define BULLET_DAMAGE_DRONE (100)




namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene(float deltaTimeSeconds, int flag);
     protected:
        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;
        ViewportArea miniViewportArea;
        
        void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int is_plane);

        typedef struct {
            glm::vec3 pos;
            bool isHittingTree(glm::vec3 posOtherTree) {
                if (pos.x + CONE_RADIUS >= posOtherTree.x - CONE_RADIUS && posOtherTree.x + CONE_RADIUS >= pos.x - CONE_RADIUS && pos.z + CONE_RADIUS >= posOtherTree.z - CONE_RADIUS && posOtherTree.z + CONE_RADIUS >= pos.z - CONE_RADIUS) {
                    return true;
                }
                else {
                    return false;
                }
            }
            bool hitbox(glm::vec3 pos2) {
                if ((pos2.x - pos.x) * (pos2.x - pos.x) + (pos2.z - pos.z) * (pos2.z - pos.z) <= TRUNK_RADIUS * TRUNK_RADIUS && (pos2.y <= TRUNK_HEIGHT && pos2.y >= 0)) {
                    return true;
                }
                else if (((pos2.x - pos.x) * (pos2.x - pos.x) + (pos2.z - pos.z) * (pos2.z - pos.z)) / ((CONE_RADIUS / CONE_HEIGHT) * (CONE_RADIUS / CONE_HEIGHT)) <=
                    ((pos2.y - (pos.y + TRUNK_HEIGHT + CONE_HEIGHT)) * (pos2.y - (pos.y + TRUNK_HEIGHT + CONE_HEIGHT))) && (pos2.y <= TRUNK_HEIGHT + CONE_HEIGHT && pos2.y >= TRUNK_HEIGHT)) {
                    return true;
                } else if (((pos2.x - pos.x) * (pos2.x - pos.x) + (pos2.z - pos.z) * (pos2.z - pos.z)) / ((CONE_RADIUS / CONE_HEIGHT) * (CONE_RADIUS / CONE_HEIGHT)) <=
                    ((pos2.y - (pos.y + TRUNK_HEIGHT + CONE_HEIGHT + TRUNK_HEIGHT / 3 * 2)) * (pos2.y - (pos.y + TRUNK_HEIGHT + CONE_HEIGHT + TRUNK_HEIGHT / 3 * 2))) &&
                    (pos2.y <= TRUNK_HEIGHT + CONE_HEIGHT + TRUNK_HEIGHT / 3 * 2 && pos2.y >= TRUNK_HEIGHT + TRUNK_HEIGHT / 3 * 2)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        } Tree;

        typedef struct {
            glm::vec3 pos;
            float propellerSpin = 0.0f;
            float speed = DRONE_SPEED;
            float rotation = 0.0f;
            float rotationSpeed = DRONE_ROTATION_SPEED;
            int health = 100;
        } Drone;

        typedef struct {
            glm::vec3 pos;
            bool delivered = true;
            bool hooked = false;
        } Package;

        typedef struct {
            glm::vec3 pos = glm::vec3(0, 0, 0);
        } CheckPoint;

        typedef struct {
            float rotation = 0.0f;
        } Arrow;

        typedef struct {
            glm::vec3 pos;
        } Cloud;

        typedef struct {
            glm::vec3 pos;
            float health = 100.0f;
            float rotation;
            float deathAnimation = 0.0f;
        } Enemy;

        enum Owner{
            DRONE, ENEMY
        };

        typedef struct {
            glm::vec3 initialPos = glm::vec3(0);
            glm::vec3 pos = glm::vec3(0);
            glm::vec3 rotation = glm::vec3(0);
            glm::vec3 direction = glm::vec3(0);
            Owner owner;
            float timeLived = 0.0f;
            bool hitDrone(glm::vec3 posDrone) {
                return (pow(pos.x - posDrone.x, 2) + pow(pos.z - posDrone.z, 2) <= pow(DRONE_WING_LENGTH / 2 + BULLET_SIZE / 2, 2) &&
                    pos.y <= posDrone.y + DRONE_WING_WIDTH  / 2 + BULLET_SIZE / 2 && pos.y >= posDrone.y - DRONE_WING_WIDTH / 2 - BULLET_SIZE / 2);
            }
            bool hitEnemy(glm::vec3 enemyPos) {
                float bulletMinX = pos.x - BULLET_SIZE / 2.0f;
                float bulletMaxX = pos.x + BULLET_SIZE / 2.0f;
                float bulletMinY = pos.y - BULLET_SIZE / 2.0f;
                float bulletMaxY = pos.y + BULLET_SIZE / 2.0f;
                float bulletMinZ = pos.z - BULLET_SIZE / 2.0f;
                float bulletMaxZ = pos.z + BULLET_SIZE / 2.0f;

                float enemyMinX = enemyPos.x - ENEMY_WIDTH / 2.0f;
                float enemyMaxX = enemyPos.x + ENEMY_WIDTH / 2.0f;
                float enemyMinY = enemyPos.y - ENEMY_HEIGHT / 2.0f;
                float enemyMaxY = enemyPos.y + ENEMY_HEIGHT / 2.0f;
                float enemyMinZ = enemyPos.z - ENEMY_WIDTH / 2.0f;
                float enemyMaxZ = enemyPos.z + ENEMY_WIDTH / 2.0f;

                bool collisionX = bulletMaxX >= enemyMinX && bulletMinX <= enemyMaxX;
                bool collisionY = bulletMaxY >= enemyMinY && bulletMinY <= enemyMaxY;
                bool collisionZ = bulletMaxZ >= enemyMinZ && bulletMinZ <= enemyMaxZ;

                return collisionX && collisionY && collisionZ;
            }
        } Bullet;

        Drone drone1;
        Package package;
        Arrow arrow;

        int points = 0;
        float timePassed = 0.0f;
        int timePassedInt = 0;

        bool thirdPerson = true;
        bool shootEnemy = false;

        std::string line1;
        std::string line2;
        std::string line3;
        std::string line4;


        std::vector<Tree> trees;
        std::vector<Drone> drones;
        std::vector<Cloud> clouds;
        std::vector<Enemy> enemies;
        std::vector<Bullet> bullets;

    };
}   // namespace m1


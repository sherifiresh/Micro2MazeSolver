using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarController : MonoBehaviour {
    
    void Awake()
    {
        string[] temp =  {
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "?????????e?????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????",
            "???????????????????"};
        //carPosX = (temp.Length) / 2; carPosY = (temp.Length)/2;
        visitedS = new int[temp.Length*temp.Length];
        refreshVS();
        visitedB = new int[temp.Length * temp.Length];
        refreshVB();
        map = temp;
        Xs = carPosX; Ys = carPosY;
        hasReachedDestination = false;
    }
    void ResetMode()//0 none, 1 move, 2 rotation
    {
        movingTimer = 0;
        movementMode = 0;
    }
    void SetMovingMode()//0 none, 1 move, 2 rotation
    {
        movingTimer = movingTimerMax;
        movementMode = 1;
    }
    void SetRotationMode(int dir, float multiplier)//0 none, 1 move, 2 rotation //-1 = left # 1 = right
    {
        rotateDir = dir;
        movingTimer = rotationTimerMax*multiplier;
        movementMode = 2;
    }
    bool isResetMode()//0 none, 1 move, 2 rotation
    {
        return movementMode == 0;
    }
    bool isMovementMode()//0 none, 1 move, 2 rotation
    {
        return movementMode == 1;
    }
    bool isRotationMode()//0 none, 1 move, 2 rotation
    {
        return movementMode == 2;
    }
    
    void refreshVS()
    {
        for (int i = 0; i < map.Length*map.Length; i++)
            visitedS[i] = 0;
    }
    void refreshVB()
    {
        for (int i = 0; i < map.Length * map.Length; i++)
            visitedB[i] = 0;
    }
    int get2DIn1D(int x, int y)
    {
        //Debug.Log("X:" + x.ToString() + " Y:" + y.ToString() + " " + (x + map.Length * y).ToString());
        return x + map.Length*y;
    }
    void Update()
    {
        if (!hasReachedDestination && !isMoving)
        {
            discover();
            pickGoToTargetSpot();            
            solve(targetX, targetY);
        }
        if (!hasReturnedToBeg && isWin() && !isMoving)
        {
            setWin();
            hasReturnedToBeg = true;
            return;
        }
        if (isMoving)
        {            
            if (instructionIndex >= instructions.Length)
            {
                ResetMode();
                isMoving = false;
            }
            else
            {
                CountDown();
                if (isResetMode())
                {
                    moveInstruction(instructions[instructionIndex]);
                }
                if (movingTimer > 0)
                {
                    if (isMovementMode())
                    {
                        MovePhysicallyForward();
                    }
                    if (isRotationMode())
                    {
                        RotatePhysically();
                    }
                }
                else
                {
                    if (!isRotationMode())
                    {
                        ResetMode();
                        instructionIndex++;
                    }
                    else
                        SetMovingMode();
                }
            }
        }
    }
    void CountDown()
    {
        movingTimer -= Time.deltaTime;
    }
    void MovePhysicallyForward()
    {
        if(facingDirection == 0) //0 up, 1 right, 2 down, 3 left
        {
            transform.position += new Vector3(0, moveSpeed * Time.deltaTime, 0);
        }
        if (facingDirection == 1) //0 up, 1 right, 2 down, 3 left
        {
            transform.position += new Vector3(moveSpeed * Time.deltaTime, 0, 0);
        }
        if (facingDirection == 2) //0 up, 1 right, 2 down, 3 left
        {
            transform.position += new Vector3(0, -moveSpeed * Time.deltaTime, 0);
        }
        if (facingDirection == 3) //0 up, 1 right, 2 down, 3 left
        {
            transform.position += new Vector3(-moveSpeed * Time.deltaTime, 0, 0);
        }
    }
    void RotatePhysically()
    {
        if(rotateDir == 1)
        {
            transform.Rotate(new Vector3(0, 0, -rotateSpeed * Time.deltaTime));
        }
        if (rotateDir == -1)
        {
            transform.Rotate(new Vector3(0, 0, rotateSpeed * Time.deltaTime));
        }
    }
    void pickGoToTargetSpot()
    {
        curPos beg = new curPos();
        beg.x = carPosX; beg.y = carPosY;
        refreshVS();
        Q.Clear();
        Q.Enqueue(beg);
        pickGoToTargetSpotBFS();
    }
    bool isWall(curPos cP)
    {
        //Opposite
        if (cP.lastDir == null || cP.lastDir.Length == 0)
            return false;
        if(cP.lastDir[cP.lastDir.Length-1] == 'u')
        {
            return map[cP.y+1][cP.x] == '1';
        }
        else if (cP.lastDir[cP.lastDir.Length - 1] == 'r')
        {
            return map[cP.y][cP.x-1] == '1';
        }
        else if (cP.lastDir[cP.lastDir.Length - 1] == 'd')
        {
            return map[cP.y-1][cP.x] == '1';
        }
        else if (cP.lastDir[cP.lastDir.Length - 1] == 'l')
        {
            return map[cP.y][cP.x+1] == '1';
        }
        return false;
    }
    void pickGoToTargetSpotBFS()
    {
        if (Q.Count == 0)
            return;
        curPos cP = Q.Peek();
        Q.Dequeue();
        if (cP.x < 0 || cP.y < 0 || cP.x >= map.Length || cP.y >= map.Length || isWall(cP) || map[cP.y][cP.x] == '?' || visitedS[get2DIn1D(cP.x, cP.y)] == 1)
        {
            pickGoToTargetSpotBFS();
            return;
        }
        visitedS[get2DIn1D(cP.x, cP.y)] = 1;
        if(visitedB[get2DIn1D(cP.x, cP.y)] == 1)
        {
            curPos right, down, left, up;
            right = new curPos();
            down = new curPos();
            left = new curPos();
            up = new curPos();
            right.x = cP.x + 2; right.y = cP.y; right.lastDir = "r";
            down.x = cP.x; down.y = cP.y + 2; down.lastDir = "d";
            left.x = cP.x - 2; left.y = cP.y; left.lastDir = "l";
            up.x = cP.x; up.y = cP.y - 2; up.lastDir = "u";
            Q.Enqueue(right); Q.Enqueue(down); Q.Enqueue(left); Q.Enqueue(up);
            pickGoToTargetSpotBFS();
            return;
        }
        Q.Clear();
        targetX = cP.x; targetY = cP.y;
        
        return;
    }
    void solve(int X, int Y)
    {
        if(X < 0 || Y < 0 || X >= map.Length || Y >= map.Length || map[Y][X] == '1' || visitedB[get2DIn1D(X, Y)] == 1)
        {
            
            return;
        }

        visitedB[get2DIn1D(X, Y)] = 1;        

        curPos beg = new curPos();
        beg.x = carPosX; beg.y = carPosY; beg.lastDir = "";
        Q.Enqueue(beg);
        refreshVS();        
        instructions = getShortestPath(X, Y);
        instructionIndex = 0;
        isMoving = true;
        ResetMode();
        //Debug.Log(directions);

        //for(int i = 0; i < directions.Length; i++)
        //{
        //    moveInstruction(directions[i]);            
        //}        

        //Debug.Log("X:" + X.ToString() + " Y:" + Y.ToString());
        
        

    }
    bool isWin()
    {
        return MazeGenerator.instance.isWin();
    }
    void discover()
    {
        //0 up, 1 right, 2 down, 3 left
        discoverDir(0);
        discoverDir(1);
        discoverDir(2);
        discoverDir(3);
    }
    void discoverDir(int dir)
    {
        char c = MazeGenerator.instance.discover(dir);
        if (dir == 0)
        {
            insertInMap(carPosX, carPosY - 1, c);
            if (c != '1')
            {
                insertInMap(carPosX, carPosY - 2, c);
            }
            
        }
        if (dir == 1)
        {
            insertInMap(carPosX + 1, carPosY, c);
            if (c != '1')
            {
                insertInMap(carPosX + 2, carPosY, c);
            }
        }
        if (dir == 2)
        {
            insertInMap(carPosX, carPosY + 1, c);
            if (c != '1')
            {
                insertInMap(carPosX, carPosY + 2, c);
            }
        }
        if (dir == 3)
        {
            insertInMap(carPosX - 1, carPosY, c);
            if (c != '1')
            {
                insertInMap(carPosX - 2, carPosY, c);
            }
        }
    }
    void insertInMap(int x, int y, char c)
    {
        char[] temp = map[y].ToCharArray();
        temp[x] = c;
        map[y] = new string(temp);
    }
    void setWin()
    {
        
        hasReachedDestination = true;
        refreshVB();
        solve(Xs, Ys);
        Debug.Log("Hurray!");
    }
    void moveInstruction(char c)
    {
        rotateTo(c);
        //0 up, 1 right, 2 down, 3 left       
        moveForward();

        if (facingDirection == 0)
        {
            carPosY -= 2;
        }
        if (facingDirection == 1)
        {
            carPosX += 2;
        }
        if (facingDirection == 2)
        {
            carPosY += 2;
        }
        if (facingDirection == 3)
        {
            carPosX -= 2;
        }
    }
    void moveForward()
    {
        MazeGenerator.instance.carMoveForward(facingDirection);
    }
    void rotateTo(char c)
    {   //0 up, 1 right, 2 down, 3 left
        int cRotation = c == 'u' ? 0 : c == 'r' ? 1 : c == 'd' ? 2 : 3;
        if (facingDirection == cRotation)
        {
            SetMovingMode();
            return;
        }
        if ((facingDirection + 1) % 4 == cRotation)
        {
            rotateRight();
            SetRotationMode(1, 1);
            return;
        }
        if (facingDirection == (cRotation + 1) % 4)
        {
            rotateLeft();
            SetRotationMode(-1, 1);
            return;
        }
        SetRotationMode(1, 2);
        rotateRight();
        rotateRight();
    }
    void rotateLeft()
    {
        facingDirection = (facingDirection + 3)%4;
    }

    void rotateRight()
    {
        facingDirection = (facingDirection + 1) % 4; ;
    }
    string getShortestPath(int tX, int tY)
    {
        if (Q.Count == 0)
            return "";
        curPos cP = Q.Peek();
        Q.Dequeue();
        if (cP.x < 0 || cP.y < 0 || cP.x >= map.Length || cP.y >= map.Length || isWall(cP) || map[cP.y][cP.x] == '?' || visitedS[get2DIn1D(cP.x, cP.y)] == 1)
        {
            
            return getShortestPath(tX, tY);
        }
        visitedS[get2DIn1D(cP.x, cP.y)] = 1;
        if (cP.x == tX && cP.y == tY)
        {
            Q.Clear();
            return cP.lastDir;
        }
        while (Q.Count != 0)
        {
            string s = getShortestPath(tX, tY);
            if (s != "")
                return s;
        }
        curPos right, down, left, up;
        right = new curPos();
        down = new curPos();
        left = new curPos();
        up = new curPos();
        right.x = cP.x + 2; right.y = cP.y; right.lastDir = cP.lastDir + "r";
        down.x = cP.x; down.y = cP.y + 2; down.lastDir = cP.lastDir + "d";
        left.x = cP.x - 2; left.y = cP.y; left.lastDir = cP.lastDir + "l";
        up.x = cP.x; up.y = cP.y - 2; up.lastDir = cP.lastDir + "u";
        Q.Enqueue(right); Q.Enqueue(down); Q.Enqueue(left); Q.Enqueue(up);

        return getShortestPath(tX, tY);
    }
    
}

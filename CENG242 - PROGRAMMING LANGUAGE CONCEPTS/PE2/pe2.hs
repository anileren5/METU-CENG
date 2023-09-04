module PE2 where

import Text.Printf

type Point = (Int, Int)
type Dimensions = (Int, Int)
type Vector = (Int, Int)

getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

castIntToDouble x = (fromIntegral x) :: Double

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------------------------------
getVector :: String -> Vector
getVector d = if d == "North" then (0,1)
              else if d == "NorthEast" then (1,1)
              else if d == "East" then (1,0)
              else if d == "SouthEast" then (1,-1)
              else if d == "South" then (0,-1)
              else if d == "SouthWest" then (-1,-1)
              else if d == "West" then (-1,0)
              else if d == "NorthWest" then (-1,1)
              else if d == "Stay" then (0,0)
              else (9,9)

-------------------------------------------------------------------------------------------------------------------------------
getAllVectors :: [String] -> [Vector]
getAllVectors l = map getVector l

-------------------------------------------------------------------------------------------------------------------------------

producePath :: Point -> [String] -> [Point]
producePath initial actions = [initial] ++ (pathConstructHelper initial actions)

-------------------------------------------------------------------------------------------------------------------------------
takePathInArea :: [Point] -> Dimensions -> [Point]
takePathInArea path (bx, by) = takePathInAreaHelper path [] (bx,by)

-------------------------------------------------------------------------------------------------------------------------------

remainingObjects :: [Point] -> Dimensions -> [Point] -> [Point]
remainingObjects path border [] = []
remainingObjects path border (x:xs) = if (not ( elem x (takePathInArea path border) ) ) then [x] ++ (remainingObjects path border xs)   
                                      else (remainingObjects path border xs)
-------------------------------------------------------------------------------------------------------------------------------
averageStepsInSuccess :: [[Point]] -> Dimensions -> [Point] -> Double
averageStepsInSuccess paths border objects = (castIntToDouble (nSteps (successfulPaths paths border objects))) / (castIntToDouble (nSuccessfulPaths paths border objects)) 





---------- Helper Functions ----------

nextPoint :: Point -> String -> Point
nextPoint (x,y) d = if d == "North" then (x,y+1)
                    else if d == "NorthEast" then (x+1,y+1) 
                    else if d == "East" then (x+1,y)
                    else if d == "SouthEast" then (x+1,y-1)
                    else if d == "South" then (x,y-1)
                    else if d == "SouthWest" then (x-1,y-1)
                    else if d == "West" then (x-1,y)
                    else if d == "NorthWest" then (x-1,y+1)
                    else if d == "Stay" then (x,y)
                    else (9,9)

pathConstructHelper :: Point -> [String] -> [Point]
pathConstructHelper initialPoint [] = []
pathConstructHelper initialPoint (x:xs) = (nextPoint initialPoint x) : (pathConstructHelper (nextPoint initialPoint x) xs)

takePathInAreaHelper :: [Point] -> [Point] -> Dimensions -> [Point]
takePathInAreaHelper [] passedPath (x,y) = passedPath -- Call with passedPath = []
takePathInAreaHelper ((x0,y0):r) passedPath (x,y) = if ((0 <= x0) && (x0 < x) && (0 <= y0) && (y0 < y)) then passedPath ++ [(x0,y0)] ++ (takePathInAreaHelper r [] (x,y))
                                                    else passedPath

isLeaving :: [Point] -> Dimensions -> Bool -- returns True if the robot leaves the area 
isLeaving path d = if (path == (takePathInArea path d)) then False
                   else True
                   
allObjectsPicked :: [Point] -> Dimensions -> [Point] -> Bool -- returns True if all objects are picked up
allObjectsPicked path d objects = if (remainingObjects path d objects) == [] then True
                                  else False
                                  
isSuccessful :: [Point] -> Dimensions -> [Point] -> Bool
isSuccessful path d objects = if (not (isLeaving path d)) && (allObjectsPicked path d objects) then True
                              else False
                              
successfulPaths :: [[Point]] -> Dimensions -> [Point] -> [[Point]]
successfulPaths [] d objects = []
successfulPaths (x:xs) d objects = if isSuccessful x d objects then [x] ++ (successfulPaths xs d objects)
                                   else (successfulPaths xs d objects)



nSuccessfulPaths :: [[Point]] -> Dimensions -> [Point] -> Int
nSuccessfulPaths paths d objects = length (successfulPaths paths d objects)

nSteps :: [[Point]] -> Int
nSteps [] = 0
nSteps (x:xs) = (length x) + (nSteps xs)















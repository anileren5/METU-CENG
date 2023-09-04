module PE1 where

import Text.Printf
import Data.List

-- Type synonyms --
type Point = (Double, Double)
type Signal = (Double, Double, Double, Double)

-- This function takes a Double and rounds it to 2 decimal places as requested in the PE --
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

getDistance :: Point -> Point -> Double
getDistance (x1, y1) (x2, y2) = getRounded(sqrt(((x2-x1)^2) + ((y2-y1)^2)))

-------------------------------------------------------------------------------------------

findAllDistances :: Point -> [Point] -> [Double]
findAllDistances b l = map (getDistance b) l

-------------------------------------------------------------------------------------------

findExtremes :: Point -> [Point] -> (Point, Point)
findExtremes b l = (findNearestPoint b l,findFarthestPoint b l)

-------------------------------------------------------------------------------------------

getSingleAction :: Signal -> String
getSingleAction (north,east,south,west) = if (north == south) && (east == west) then "Stay"
                                          else if (north > south) && (east == west) then "North"
                                          else if (south > north) && (east == west) then "South"
                                          else if (north == south) && (east > west) then "East"
                                          else if (north == south) && (west > east) then "West"
                                          else if (north > south) && (east > west) then "NorthEast"
                                          else if (north > south) && (west > east) then "NorthWest"
                                          else if (south > north) && (east > west) then "SouthEast"
                                          else if (south > north) && (west > east) then "SouthWest"
                                          else "Error!"

-------------------------------------------------------------------------------------------

getAllActions :: [Signal] -> [String]
getAllActions signals = map getSingleAction signals

-------------------------------------------------------------------------------------------

numberOfGivenAction :: Num a => [Signal] -> String -> a
numberOfGivenAction signals action = fromIntegral (length (filter (checkAction action) (getAllActions signals)))


-- Helper Functions

findMinHelper :: [Double] -> Double -> Double -- Pass the first element of the list as a second parameter while calling the function.
findMinHelper [] c = c
findMinHelper (x:xs) c = if x > c then findMinHelper xs c
                   else findMinHelper xs x
                   
findMaxHelper :: [Double] -> Double -> Double -- Pass the first element of the list as a second parameter while calling the function.
findMaxHelper [] c = c
findMaxHelper (x:xs) c = if x < c then findMaxHelper xs c
                   else findMaxHelper xs  x

findMin :: [Double] -> Double
findMin l = findMinHelper l (l!!0)

findMax :: [Double] -> Double
findMax l = findMaxHelper l (l!!0)

findMinIndexHelper :: [Double] -> Int -> Int -- Past 0 as a second parameter while calling the function
findMinIndexHelper (x:xs) i = if x == findMin((x:xs)) then i
                        else findMinIndexHelper xs (i+1)
                
findMinIndex :: [Double] -> Int
findMinIndex l = (findMinIndexHelper l 0)

findMaxIndexHelper :: [Double] -> Int -> Int
findMaxIndexHelper (x:xs) i = if x == findMax((x:xs)) then i
                   else findMaxIndexHelper xs (i+1)

findMaxIndex :: [Double] -> Int
findMaxIndex l = (findMaxIndexHelper l 0)


findNearestPoint :: Point -> [Point] -> Point
findNearestPoint b l = l !! (findMinIndex (findAllDistances b l)) 

findFarthestPoint :: Point -> [Point] -> Point
findFarthestPoint b l = l !! (findMaxIndex (findAllDistances b l))

checkAction :: String -> String -> Bool
checkAction requiredAction takenAction = (requiredAction == takenAction)




























                   
                   


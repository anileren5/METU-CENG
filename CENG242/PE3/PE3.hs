module PE3 where

-- can use these if you want to...
import Data.List
import Data.Maybe

data Private = Private { idNumber :: Int, height :: Int, timeToSwap :: Int } deriving Show

data Cell = Empty | Full Private deriving Show

type Area = [[Cell]] 

---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------


-- Note: undefined is a value that causes an error when evaluated. Replace it with
-- a viable definition! Name your arguments as you like by changing the holes: _


--------------------------
-- Part I: Warming up with Abstractions

-- bubbleHumans: Applying a parallel bubble step on integers.
bubbleHumans :: [Int] -> [Int]
bubbleHumans l = bubbleHumansHelper l

-- bubblePrivates: The same thing, but on privates with a key function and an asc/desc option.
bubblePrivates :: (Private -> Int) -> Bool -> [Private] -> [Private]
bubblePrivates key order privs = bubblePrivatesHelperFull key order privs

-- sortPrivatesByHeight: Full sort via multiple bubble steps, calculate the sorting time too!
sortPrivatesByHeight :: [Private] -> ([Private], Int)
sortPrivatesByHeight privs = manyStepsDone privs


--------------------------
-- Part II: Squeezing your Brain Muscles

-- ceremonialFormation: Sorting again, but with multiple files. Don't forget the time!
ceremonialFormation :: Area -> (Area, Int)
ceremonialFormation area = ceremonialFormationHelper area

-- swapPrivates: Swap two arbitrary privates by ID if they are in the area. Big ouch!
swapPrivates :: Int -> Int -> Area -> Area
swapPrivates id1 id2 area = swapPrivatesHelper id1 id2 area 






-- Best of luck to you, friend and colleague!



----------Helper Functions ----------

bubbleHumansHelper :: [Int] -> [Int]
bubbleHumansHelper [] = []
bubbleHumansHelper [x] = [x]
bubbleHumansHelper (x:(y:r)) = if x >= y then [x] ++ bubbleHumansHelper (y:r)
                               else [y,x] ++ (bubbleHumansHelper r)
                               
                               
                               
                                   

bubblePrivatesHelperDescending :: (Private -> Int) -> [Private] -> [Private]
bubblePrivatesHelperDescending _ [] = []
bubblePrivatesHelperDescending _ [x] = [x]
bubblePrivatesHelperDescending key (x:(y:r)) = if (key x) >= (key y) then [x] ++ bubblePrivatesHelperDescending key (y:r)
                                               else [y,x] ++ (bubblePrivatesHelperDescending key r)

bubblePrivatesHelperAscending :: (Private -> Int) -> [Private] -> [Private]
bubblePrivatesHelperAscending _ [] = []
bubblePrivatesHelperAscending _ [x] = [x]
bubblePrivatesHelperAscending key (x:(y:r)) = if (key x) <= (key y) then [x] ++ bubblePrivatesHelperAscending key (y:r)
                                              else  [y,x] ++ (bubblePrivatesHelperAscending key r)

bubblePrivatesHelperFull :: (Private -> Int) -> Bool -> [Private] -> [Private]
bubblePrivatesHelperFull key order privs = if order == True then bubblePrivatesHelperDescending key privs
                                           else bubblePrivatesHelperAscending key privs
                             







oneStep :: [Private] -> ([Private],Int) -- call with curmax = 0 !!!!!
oneStep privs = (oneStepBubble privs , oneStepTimes privs)
                        
oneStepDone :: [Private] -> ([Private],Int)
oneStepDone privs = oneStep privs

manySteps :: [Private] -> Int -> Int -> ([Private],Int) -- call with times = sqr(len privs) and accumulator =  0
manySteps privs 0 accumulator = (privs,accumulator) 
manySteps privs times accumulator = manySteps (fst (oneStepDone privs)) (times - 1) (accumulator + snd (oneStepDone privs))

manyStepsDone :: [Private] -> ([Private],Int)
manyStepsDone privs = manySteps privs ((length privs)^2) 0


oneStepBubble :: [Private] -> [Private] 
oneStepBubble privs = bubblePrivatesHelperDescending height privs

oneStepTimesHelper :: [Private] -> Int -> Int
oneStepTimesHelper [] curmax = curmax
oneStepTimesHelper [x] curmax = curmax
oneStepTimesHelper (x:(y:r)) curmax = if height x >= height y then oneStepTimesHelper (y:r) curmax
                                      else oneStepTimesHelper (r) (max curmax (max (timeToSwap x) (timeToSwap y)))

oneStepTimes :: [Private] -> Int
oneStepTimes privs = oneStepTimesHelper privs 0


oneStepBubbleFormation :: [Cell] -> [Cell]
oneStepBubbleFormation [] = []
oneStepBubbleFormation [x] = [x]
oneStepBubbleFormation (Empty:(Empty:r)) = [Empty] ++ oneStepBubbleFormation (Empty:r)
oneStepBubbleFormation (Empty:(Full x:r)) = [Full x, Empty] ++ oneStepBubbleFormation r
oneStepBubbleFormation (Full x:(Empty:r)) = [Full x] ++ oneStepBubbleFormation (Empty:r)
oneStepBubbleFormation (Full x :(Full y :r)) = if (height x) >= (height y) then [Full x] ++ oneStepBubbleFormation((Full y:r))
                                               else [Full y, Full x] ++ oneStepBubbleFormation r

manyOneStepBubbleFormation :: [Cell] -> Int -> [Cell] -- call with times = len^2
manyOneStepBubbleFormation file 0 = file
manyOneStepBubbleFormation file times = manyOneStepBubbleFormation (oneStepBubbleFormation file) (times - 1)


singleFileFormation :: [Cell] -> [Cell]
singleFileFormation file = manyOneStepBubbleFormation file ((length file)^2)

oneStepTimesFormation :: [Cell] -> Int -> Int -- call with curmax = 0
oneStepTimesFormation [] curmax = curmax
oneStepTimesFormation [_] curmax = curmax
oneStepTimesFormation (Empty:(Empty:r)) curmax =  max curmax (oneStepTimesFormation (Empty:r) curmax)
oneStepTimesFormation (Empty:(Full x:r)) curmax = max (max curmax (timeToSwap x)) (oneStepTimesFormation r curmax)
oneStepTimesFormation (Full x:(Empty:r)) curmax = max curmax (oneStepTimesFormation (Empty:r) curmax)
oneStepTimesFormation (Full x:(Full y:r)) curmax = if (height x >= height y) then max curmax (oneStepTimesFormation (Full y:r) curmax)
                                                   else max (max curmax (max (timeToSwap x) (timeToSwap y))) (oneStepTimesFormation r curmax) 

oneStepTimesFormationDone :: [Cell] -> Int
oneStepTimesFormationDone file = oneStepTimesFormation file 0

totalFormationTime :: [Cell] -> Int -> Int -- acc = 0 times = l^2
totalFormationTime file 0 = 0
totalFormationTime file times = (oneStepTimesFormationDone file) + (totalFormationTime (oneStepBubbleFormation file) (times - 1))

totalFormationTimeDone :: [Cell] -> Int
totalFormationTimeDone file = totalFormationTime file ((length file)^2) 

areaFormation :: Area -> Area
areaFormation area = map singleFileFormation area

filesFormationTimes :: Area -> [Int]
filesFormationTimes area = map totalFormationTimeDone area


areaFormationTime :: Area -> Int
areaFormationTime area = maximum (filesFormationTimes area)

ceremonialFormationHelper :: Area -> (Area,Int)
ceremonialFormationHelper area = (areaFormation area, areaFormationTime area)








isExistInFile :: Int -> [Cell] -> Bool 
isExistInFile n [] = False 
isExistInFile n (Empty:xs) = isExistInFile n xs
isExistInFile n (Full x:xs) = if idNumber x == n then True 
                              else isExistInFile n xs

isExistInArea :: Int -> Area -> Bool 
isExistInArea n [] = False 
isExistInArea n (x:xs) = if isExistInFile n x then True
                         else isExistInArea n xs

isExistInSameFile :: Int -> Int -> Area -> Bool -- Scans whole area
isExistInSameFile id1 id2 [] = False 
isExistInSameFile id1 id2 (x:xs) = if (isExistInFile id1 x) && (isExistInFile id2 x) then True 
                                   else isExistInSameFile id1 id2 xs

givePrivateWithId :: Int -> [Cell] -> Cell
givePrivateWithId id file = [x | x <- file, f id x]!!0

giveCellWithIdFromArea :: Int -> Area -> Cell
giveCellWithIdFromArea id [] = Empty
giveCellWithIdFromArea id (x:xs) = if isExistInFile id x then givePrivateWithId id x
                                   else giveCellWithIdFromArea id xs 


f :: Int -> Cell -> Bool
f id Empty = False 
f id (Full x) = ((idNumber x) == id) 

put :: Int -> Cell -> [Cell] -> [Cell]
put id1 copied [] = []
put id1 copied (Empty:xs) = [Empty] ++ (put id1 copied xs)
put id1 copied (Full x:xs) = if (idNumber x) == id1 then [copied] ++ xs
                             else [Full x] ++ (put id1 copied xs)

putpartial :: Int -> Int -> [Cell] -> [Cell]
putpartial id1 id2 file = put id1 (givePrivateWithId id2 file) file

putfull :: Int -> Int -> [Cell] -> [Cell]

putfull id2 id1 file = if (indexOfWithId id2 file (-1)) <= (indexOfWithId id1 file (-1)) then put id2 (givePrivateWithId id1 file) (putpartial id1 id2 file)
                       else put id1 (givePrivateWithId id2 file) (putpartial id2 id1 file)


swapInCaseSameFile :: Int -> Int -> [[Cell]] -> [[Cell]]
swapInCaseSameFile id1 id2 [] = []
swapInCaseSameFile id1 id2 (x:xs) = if (isExistInFile id1 x) && (isExistInFile id2 x) then [(putfull id1 id2 x)] ++ xs
                                    else [x] ++ swapInCaseSameFile id1 id2 xs

indexOfWithId :: Int -> [Cell] -> Int -> Int
indexOfWithId id [] i = -1
indexOfWithId id (Full x:xs) i = if id == (idNumber x) then (i+1)
                                 else indexOfWithId id xs (i+1)
indexOfWithId id (Empty : xs) i = indexOfWithId id xs (i+1)


indexOfFile :: Int -> Area -> Int -> Int
indexOfFile id [] i = -1
indexOfFile id (x:xs) i = if isExistInFile id x then (i+1)
                          else indexOfFile id xs (i+1)


swapInCaseDifferentFile :: Int -> Int -> Cell -> Cell -> Area -> Area
swapInCaseDifferentFile id1 id2 cell1 cell2 [] = []
swapInCaseDifferentFile id1 id2 cell1 cell2 (x:xs) = if (isExistInFile id1 x) then [(put id1 cell2 x)] ++ swapInCaseDifferentFile id1 id2 cell1 cell2 xs
                                                     else if (isExistInFile id2 x) then [(put id2 cell1 x)] ++ swapInCaseDifferentFile id1 id2 cell1 cell2 xs
                                                     else [x] ++ (swapInCaseDifferentFile id1 id2 cell1 cell2 xs)




swapPrivatesHelper :: Int -> Int -> Area -> Area
swapPrivatesHelper id1 id2 area = if ((isExistInArea id1 area) == False) || ((isExistInArea id2 area) == False) then area
                                  else if isExistInSameFile id1 id2 area  then swapInCaseSameFile id1 id2 area
                                  else swapInCaseDifferentFile id1 id2  (giveCellWithIdFromArea id1 area) (giveCellWithIdFromArea id2 area) area



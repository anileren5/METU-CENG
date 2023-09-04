module PE4 where

import Data.List
import Data.Maybe

data Room = SeedStorage Int
          | Nursery Int Int
          | QueensChambers
          | Tunnel
          | Empty
          deriving Show

data Nestree = Nestree Room [Nestree] deriving Show

---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------

-- Note: undefined is a value that causes an error when evaluated. Replace it with
-- a viable definition! Name your arguments as you like by changing the holes: _

---------------------------------------------------------------------------------------------

-- Q1: Calculate the nutrition value of a given nest.
nestNutritionValue :: Nestree -> Int
nestNutritionValue nest = nestNutritionValueHelper 0 nest

-- Q2: Calculate the nutrition value of each root-to-leaf path.
pathNutritionValues :: Nestree -> [Int]
pathNutritionValues nest = pathNutritionValuesHelper 0 [] nest

-- Q3: Find the depth of the shallowest tunnel, if you can find one.
shallowestTunnel :: Nestree -> Maybe Int
shallowestTunnel nest = if (isTunnelExist False nest) then Just (shallowestTunnelHelper 1 nest)
                        else Nothing

-- Q4: Find the path to the Queen's Chambers, if such a room exists.
pathToQueen :: Nestree -> Maybe [Room]
pathToQueen nest = if (isQueensChambersExist False nest) then Just (pathToQueenHelper [] nest)
                   else Nothing 

-- Q5: Find the quickest depth to the Queen's Chambers, including tunnel-portation :)
quickQueenDepth :: Nestree -> Maybe Int
quickQueenDepth nest = quickQueenDepthHelper nest

-- Example nest given in the PDF.
exampleNest :: Nestree
exampleNest = 
  Nestree Tunnel [
    Nestree (SeedStorage 15) [
      Nestree (SeedStorage 81) []
    ],
    Nestree (Nursery 8 16) [
      Nestree Tunnel [
        Nestree QueensChambers [
          Nestree (Nursery 25 2) []
        ]
      ]
    ],
    Nestree Tunnel [
      Nestree Empty [],
      Nestree (SeedStorage 6) [
        Nestree Empty [],
        Nestree Empty []
      ]
    ]
  ]

-- Same example tree, with tunnels replaced by Empty
exampleNestNoTunnel :: Nestree
exampleNestNoTunnel = 
  Nestree Empty [
    Nestree (SeedStorage 15) [
      Nestree (SeedStorage 81) []
    ],
    Nestree (Nursery 8 16) [
      Nestree Empty [
        Nestree QueensChambers [
          Nestree (Nursery 25 2) []
        ]
      ]
    ],
    Nestree Empty [
      Nestree Empty [],
      Nestree (SeedStorage 6) [
        Nestree Empty [],
        Nestree Empty []
      ]
    ]
  ]




---------- Helper Functions ----------

nestNutritionValueHelper :: Int -> Nestree -> Int -- call with acc = 0
nestNutritionValueHelper acc (Nestree Empty []) = acc
nestNutritionValueHelper acc (Nestree (SeedStorage s) []) = acc + (s*3)
nestNutritionValueHelper acc (Nestree (Nursery e l) [])  = acc + (e*7) + (l*10)
nestNutritionValueHelper acc (Nestree Tunnel []) = acc
nestNutritionValueHelper acc (Nestree QueensChambers [])  = acc
nestNutritionValueHelper acc (Nestree Tunnel deepers) = acc + (sum (map (nestNutritionValueHelper 0) deepers))
nestNutritionValueHelper acc (Nestree QueensChambers deepers) = acc + (sum (map (nestNutritionValueHelper 0) deepers))
nestNutritionValueHelper acc (Nestree (SeedStorage s) deepers) = acc + (s*3) + (sum (map (nestNutritionValueHelper 0) deepers))
nestNutritionValueHelper acc (Nestree (Nursery e l) deepers) = acc + (e*7) + (l*10) + (sum (map (nestNutritionValueHelper 0) deepers))
nestNutritionValueHelper acc (Nestree Empty deepers) = acc + (sum (map (nestNutritionValueHelper 0) deepers))




pathNutritionValuesHelper :: Int -> [Int] -> Nestree -> [Int] -- call with acc = 0 and values = []
pathNutritionValuesHelper acc values (Nestree Empty []) = values ++ [acc]
pathNutritionValuesHelper acc values (Nestree (SeedStorage s) []) = values ++ [(acc + (s*3))]
pathNutritionValuesHelper acc values (Nestree (Nursery e l) []) = values ++ [(acc + (e*7) + (l*10))]
pathNutritionValuesHelper acc values (Nestree Tunnel []) = values ++ [acc]
pathNutritionValuesHelper acc values (Nestree QueensChambers []) = values ++ [acc]
pathNutritionValuesHelper acc values (Nestree Empty deepers) = values ++ (concat (map (pathNutritionValuesHelper acc []) deepers))
pathNutritionValuesHelper acc values (Nestree (SeedStorage s) deepers) = values ++ (concat (map (pathNutritionValuesHelper (acc + (s*3)) []) deepers))
pathNutritionValuesHelper acc values (Nestree (Nursery e l) deepers) = values ++ (concat (map (pathNutritionValuesHelper (acc + (e*7)+(l*10)) []) deepers))
pathNutritionValuesHelper acc values (Nestree Tunnel deepers) = values ++ (concat (map (pathNutritionValuesHelper acc []) deepers))
pathNutritionValuesHelper acc values (Nestree QueensChambers deepers) = values ++ (concat (map (pathNutritionValuesHelper acc []) deepers))




greatOr :: [Bool] -> Bool 
greatOr [] = False 
greatOr (x:xs) = x || (greatOr xs)


isTunnelExist :: Bool -> Nestree -> Bool -- call with b = False
isTunnelExist b (Nestree Empty [])  = b
isTunnelExist b (Nestree (SeedStorage _) [])  = b 
isTunnelExist b (Nestree (Nursery _ _) [])  = b 
isTunnelExist b (Nestree QueensChambers [])  = b
isTunnelExist b (Nestree Empty deepers)  = greatOr (map (isTunnelExist b) deepers)
isTunnelExist b (Nestree (SeedStorage s) deepers)  = greatOr (map (isTunnelExist b) deepers)
isTunnelExist b (Nestree (Nursery e l) deepers)  = greatOr (map (isTunnelExist b) deepers)
isTunnelExist b (Nestree QueensChambers deepers)  = greatOr (map (isTunnelExist b) deepers)
isTunnelExist b (Nestree Tunnel deepers)  = True


shallowestTunnelHelper :: Int -> Nestree -> Int -- call with curlev = 1
shallowestTunnelHelper curlev (Nestree Tunnel _) = curlev
shallowestTunnelHelper curlev (Nestree Empty []) = 2147483647
shallowestTunnelHelper curlev (Nestree (SeedStorage _) []) = 2147483647
shallowestTunnelHelper curlev (Nestree (Nursery _ _) []) = 2147483647
shallowestTunnelHelper curlev (Nestree QueensChambers []) = 2147483647
shallowestTunnelHelper curlev (Nestree Empty deepers) = minimum (map (shallowestTunnelHelper (curlev+1)) deepers)
shallowestTunnelHelper curlev (Nestree (SeedStorage _) deepers) = minimum (map (shallowestTunnelHelper (curlev+1)) deepers)
shallowestTunnelHelper curlev (Nestree (Nursery _ _) deepers) = minimum (map (shallowestTunnelHelper (curlev+1)) deepers)
shallowestTunnelHelper curlev (Nestree QueensChambers deepers) = minimum (map (shallowestTunnelHelper (curlev+1)) deepers)





isQueensChambersExist :: Bool -> Nestree -> Bool -- call with b = False
isQueensChambersExist b (Nestree Empty []) = b
isQueensChambersExist b (Nestree (SeedStorage _) []) = b
isQueensChambersExist b (Nestree (Nursery _ _) []) = b
isQueensChambersExist b (Nestree Tunnel []) = b
isQueensChambersExist b (Nestree Empty deepers) = greatOr (map (isQueensChambersExist b) deepers)
isQueensChambersExist b (Nestree (SeedStorage _) deepers) = greatOr (map (isQueensChambersExist b) deepers)
isQueensChambersExist b (Nestree (Nursery _ _) deepers) = greatOr (map (isQueensChambersExist b) deepers)
isQueensChambersExist b (Nestree Tunnel deepers) = greatOr (map (isQueensChambersExist b) deepers)
isQueensChambersExist b (Nestree QueensChambers deepers) = True


pathToQueenHelper :: [Room] -> Nestree -> [Room] -- call with curpath = []
pathToQueenHelper curpath (Nestree QueensChambers deepers) = curpath
pathToQueenHelper curpath (Nestree Empty []) = [] 
pathToQueenHelper curpath (Nestree (SeedStorage _) []) = []
pathToQueenHelper curpath (Nestree (Nursery _ _) []) = [] 
pathToQueenHelper curpath (Nestree Tunnel []) = []
pathToQueenHelper curpath (Nestree Empty deepers) = concat (map (pathToQueenHelper (curpath ++ [Empty])) deepers)
pathToQueenHelper curpath (Nestree (SeedStorage s) deepers) = concat (map (pathToQueenHelper (curpath ++ [SeedStorage s])) deepers)
pathToQueenHelper curpath (Nestree (Nursery e l) deepers) = concat (map (pathToQueenHelper (curpath ++ [Nursery e l])) deepers)
pathToQueenHelper curpath (Nestree Tunnel deepers) = concat (map (pathToQueenHelper (curpath ++ [Tunnel])) deepers)


isTunnelExistOnThePath :: [Room] -> Bool
isTunnelExistOnThePath [] = False 
isTunnelExistOnThePath (Tunnel:xs) = True 
isTunnelExistOnThePath (_:xs) = isTunnelExistOnThePath xs 


quickQueenDepthHelper :: Nestree -> Maybe Int 
quickQueenDepthHelper nest =  if (isQueensChambersExist False nest) == False then Nothing 
                               else if (isTunnelExistOnThePath (pathToQueenHelper [] nest)) == False then Just ((length (pathToQueenHelper [] nest)) + 1)
                               else Just (((shallowestTunnelHelper 1 nest) + (length (rest (pathToQueenHelper [] nest)))) + 1)


rest :: [Room] -> [Room]
rest [] = []
rest (Tunnel:r) = if (isTunnelExistOnThePath r) == False then r
                  else rest r
rest (_:r) = rest r



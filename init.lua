local matrixOp = require("gizmo_matrix")


local matrixFormat = "[" .. ((" % 08.6f"):rep(4) .. "\n "):rep(4):sub(0, -3) .. " ]"

local function dumpMatrix(matrix)
	return matrixFormat:format(table.unpack(matrix))
end

local vectorFormat = "(" .. (("% 12.6f, "):rep(3)):sub(0, -3) .. ")"

local function dumpVector(vector)
	return vectorFormat:format(table.unpack(vector))
end

local iTranslate, iRotate, iScale =
	{ 0.03326626, 1.2155993, 0.031883594 },
	{ 90, 90.00001, 0 },
	{ 0.99999976, 0.99999976, 0.99999976, 0.99999595 }

print("iTranslate = " .. dumpVector(iTranslate))
print("iRotate    = " .. dumpVector(iRotate))
print("iScale     = " .. dumpVector(iScale))

print()

local matrix = matrixOp.recompose(iTranslate, iRotate, iScale)

print(dumpMatrix(matrix))
print()

local translate, rotate, scale = matrixOp.decompose(matrix)

print("translate = " .. dumpVector(translate))
print("rotate    = " .. dumpVector(rotate))
print("scale     = " .. dumpVector(scale))
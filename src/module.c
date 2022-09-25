#include "MatrixOps.h"
#include <lua.h>

#include <f17/common.h>
#include <f17/lua.h>

unit void check_float_tab(lua_State* L, int arg, int count, float* target) {
	char buf[0x80];

	luaL_checktype(L, arg, LUA_TTABLE);
	if(luaL_len(L, 1) < count) {
		snprintf(buf, sizeof(buf), "expected table of %d numbers", count);
		luaL_argerror(L, arg, buf);
	}

	for(int i = 1; i <= count; i++) {
		if(lua_geti(L, arg, i) != LUA_TNUMBER) {
			snprintf(buf, sizeof(buf), "index %d was not a number", i);
			luaL_argerror(L, arg, buf);
		}
		*target = (float) lua_tonumber(L, -1);
		target++;
		lua_pop(L, 1);
	}
}

unit void floats_to_lua(lua_State* L, const float* source, int count) {
	lua_createtable(L, count, 0);
	for(int i = 1; i <= count; i++) {
		lua_pushnumber(L, (lua_Number) *source);
		lua_seti(L, -2, i);
		source++;
	}
}

unit lua_callable int decompose_wrapper(lua_State* L) {

	float matrix[16];
	float translate[3];
	float rotate[3];
	float scale[3];

	check_float_tab(L, 1, 16, matrix);

	DecomposeMatrixToComponents(matrix, translate, rotate, scale);

	floats_to_lua(L, translate, 3);
	floats_to_lua(L, rotate, 3);
	floats_to_lua(L, scale, 3);
	return 3;
}

unit lua_callable int recompose_wrapper(lua_State* L) {
	float translate[3];
	float rotate[3];
	float scale[3];
	float matrix[16];

	check_float_tab(L, 1, 3, translate);
	check_float_tab(L, 2, 3, rotate);
	check_float_tab(L, 3, 3, scale);

	RecomposeMatrixFromComponents(translate, rotate, scale, matrix);

	floats_to_lua(L, matrix, 16);
	return 1;

}

int luaopen_gizmo_matrix(lua_State* L) {
	lua_createtable(L, 0, 2);
	lua_pushcfunction(L, &decompose_wrapper);
	lua_setfield(L, -2, "decompose");
	lua_pushcfunction(L, &recompose_wrapper);
	lua_setfield(L, -2, "recompose");
	return 1;
}


//
//  main4.cpp
//  nanoVGTest
//
//  Created by Hyun Joon Shin on 6/7/24.
//

#include <stdio.h>

#define JGL2_IMPLEMENTATION
#include <JGL2/JGL.hpp>
#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <JGL2/JGL.hpp>
#include <JGL2/Simulation3DView.hpp>
#include <bits/stdc++.h>

using namespace JGL2;
using namespace jm;

ImageViewer* view = nullptr;

const int GRID_W = 256;
const int GRID_H = 256;

float* r = nullptr;
jm::vec3* u = nullptr;
bool pushed = false;
jm::vec2 cursorPt;

template<typename T>
T sample(T* r, float x, float y, int w, int h) {
	int xi = int(floor(x));
	int yi = int(floor(y)); 
	float xt = x - xi;
	float yt = y - yi;
	int x1 = min(max(0, xi), w - 1);
	int x2 = min(max(0, xi+1), w - 1);
	int y1 = min(max(0, yi), h - 1);
	int y2 = min(max(0, yi+1), h - 1);
	return mix(mix(r[y1 * w + x1], r[y1 * w + x2], xt), mix(r[y2 * w + x1], r[y2 * w + x2], xt), yt);
}

template<typename T>
void advection(T* r, int w, int h, jm::vec3* u, float dt) {
	T* tmp = new T[w * h];
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
		tmp[y * w + x] = sample(r, x - u[y * w + x].x*dt, y - u[y * w + x].y*dt, w, h);
	}
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++)
		r[y * w + x] = tmp[y * w + x];
	delete[]tmp;
}

template<typename T>
void diffuse(T* r, int w, int h, float k, float dt) {
	float kdt = k * dt;
	for (int iter = 0; iter < 5; iter++) {
		for (int y = 1; y < h - 1; y++) for (int x = 1; x < w - 1; x++) {
			int idx = y * GRID_W + x;
			r[idx] = (r[idx] + kdt * (r[x + 1 + y * w] + r[x - 1 + y * w] + r[x + (y + 1) * w] + r[x + (y - 1) * w])) / (1 + 4 * kdt);
		}
	}
}

void init() {
	if(r) delete r;
	r = new float[GRID_W*GRID_H];
	if (u) delete u; 
	u = new jm::vec3[GRID_W*GRID_H];
	for( int i=0; i<GRID_W*GRID_H; i++) { r[i]=0; u[i]=jm::vec3(0); }
} 

void conserveMass(vec3* u, int w, int h) {
	float* p = new float[w * h];
	for (int i = 0; i < w * h; i++) p[i] = 0;

	for (int iter = 0; iter < 10; iter++) {
		for (int y = 1; y < h - 1; y++) for (int x = 1; x < w - 1; x++) {
				int i = x + y * w;
				p[i] = ((u[i + 1].x - u[i - 1].x + u[i + w].y - u[i - w].y) - (p[i - 1] + p[i + 1] + p[i - w] + p[i + w])) * -0.25f;
		}
	}

	for (int y = 1; y < h - 1; y++) for (int x = 1; x < w - 1; x++) {
			int i = x + y * w;
			u[i].xy -= vec2(p[i + 1] - p[i - 1], p[i + w] - p[i - w]) / 2.0f;
	}

	delete[] p;
}

void draw(const rct_t&) {
	if( pushed && cursorPt.x>=0 && cursorPt.y>=0 && cursorPt.x<GRID_W && cursorPt.y<GRID_H)
		r[int(cursorPt.x)+int(cursorPt.y)*GRID_W]+=10;

	diffuse(r, GRID_W, GRID_H, 3, 0.01666);
	advection(r, GRID_W, GRID_H, u, 0.0166);

	diffuse(u, GRID_W, GRID_H, 10, 0.01666);
	advection(u, GRID_W, GRID_H, u, 0.0166);
	conserveMass(u, GRID_W, GRID_H);

	view->setImage((float*)r, GRID_W, GRID_H, 1, false, false);
//	view->redraw();
	view->animate();
}

bool push(button_t btn) {
	pushed = true;
	return true;
}

bool release(button_t btn) {
	pushed = false;
	return true;
}

bool drag(const jm::vec2& pr) {
	cursorPt = view->screenToImage(pr);
	return true;
}

bool move(const jm::vec2& pr) {
	auto pt = view->screenToImage(pr);
	if( _JGL::eventMods(mod_t::SHIFT) ) {
		auto d = pt - cursorPt;
		if( cursorPt.x>=0 && cursorPt.y>=0 && cursorPt.x<GRID_W && cursorPt.y<GRID_H)
			u[int(cursorPt.x)+int(cursorPt.y)*GRID_W]+=vec3(d*100, 0 );
	}
	cursorPt = pt;
	
	return true;
}

bool key(int k) {
	if (k == '0') {
		init();
		return true;
	}
	return false;
}

int main(int argc, const char * argv[]) {
	Window* win = new Window(800,600,"IK");
	view = new ImageViewer(0,0,800,600,"View");
	view->keyCB(key);
	view->drawGLCB(draw);
	view->moveCB(move);
	view->pushCB(push);
	view->releaseCB(release);
	view->dragCB(drag);
	init();
	win->show();
	_JGL::run();
	
}

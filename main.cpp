#include <iostream>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <chrono>
#include <cstdio>
#ifdef __ANDROID__

#include <jni.h>
#include <android/native_activity.h>
#include <android/log.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2platform.h>
#include <GLES2/gl2ext.h>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Mysfml", __VA_ARGS__))

#endif
#ifndef __ANDROID__
#include <GL/gl.h>
#endif
#define W 1920
#define H 1080
#define scale 30.
#define fW(x) x*scale
#define tW(x) x/scale
#define deg 57.59577
#define DEBUG 1
class World
{
	World();
	~World();

};
enum btype
{
	box=1,
	player
};
b2Vec2 gravity(0., 9.8);
b2World world(gravity);
using namespace std;
using namespace sf;
using namespace ImGui;

void setWall(int x, int y, int w, int h)
{
	b2PolygonShape gr;
	gr.SetAsBox(w/scale, h/scale);

	b2BodyDef bdef;
	bdef.position.Set(x/scale, y/scale);
	b2Body *ground = world.CreateBody(&bdef);
	ground->CreateFixture(&gr,1.);
	cout <<ground->GetPosition().x*scale <<" " <<ground->GetPosition().y*scale <<endl;
}

b2Body *inttody(int x, int y, int w, int h, btype data)
{
	b2PolygonShape sh;
	sh.SetAsBox(w/scale, h/scale);

	b2BodyDef bdef;
	bdef.position.Set(x/scale, y/scale);
	bdef.type = b2_dynamicBody;
	bdef.userData.pointer = data;

	for(int i=0; i<200; i++)
	{
	b2Body *body = world.CreateBody(&bdef);
	b2FixtureDef bfix;
	bfix.shape = &sh;
	//bfix.restitution = 0.6;
	bfix.density = 3;
	body->CreateFixture(&bfix);
	//body->SetUserData((void*)data);
	}
	return 0;
}
b2Body *intplayes(int x, int y, int r,  btype data)
{
	b2CircleShape sh;
	sh.m_radius = r/scale;

	b2BodyDef bdef;
	bdef.position.Set(x/scale, y/scale);
	bdef.type = b2_dynamicBody;
	bdef.userData.pointer = data;

	b2Body *body = world.CreateBody(&bdef);
	b2FixtureDef bfix;
	bfix.shape = &sh;
	bfix.restitution = 0.4;
	bfix.density = 20;
	body->CreateFixture(&bfix);
	//body->SetUserData((void*)data);

	return body;
}
int main(int , char* [])
{
#ifdef __ANDROID__
	LOGE("started");
	RenderWindow app(VideoMode(VideoMode::getDesktopMode()), "Box2d");
#endif
#ifndef __ANDROID__
	ContextSettings ctx;
	ctx.antialiasingLevel = 16;
	RenderWindow app(VideoMode(W, H), "Box2d", Style::Fullscreen, ctx);
#endif
	Texture t1, t2, t3;
	t1.loadFromFile("images/crate_43.png");
	t2.loadFromFile("images/gorilla.png");
	t3.loadFromFile("images/slice01_01.png");
//	t3.setRepeated(true);
	SFML::Init(app);
#ifdef __ANDROID__
	GetStyle().ScaleAllSizes(5);
	GetIO().FontGlobalScale = 2.5;
#endif
	unsigned int texr = t3.getNativeHandle();
	glBindTexture(GL_TEXTURE_2D, texr);
	if(glGetError ()) return -1;;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(glGetError ()) return -1;;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if(glGetError ()) return -1;;

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		if(glGetError ()) return -1;;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x2900);
		if(glGetError ()) return -1;;
//	t3.setRepeated(true);
	t3.setSmooth(true);
	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite gr(t3, IntRect(0 ,0, 70*1200, 70)),//Horizontall wall: 70 - width, 120 times
			vgr(t3, IntRect(0 ,0, 70, 70*20)),//Verticall wall: 70 - height, 20 times
			play(t2),
			box(t1);
	box.setOrigin(32, 32);
	play.setOrigin(64, 64);
	gr.setPosition(0, H-70);
	vgr.setPosition(0, H-70*20);
	box.setOrigin(box.getLocalBounds().width/2., box.getLocalBounds().width/2.);

	View view(Vector2f(W/2,H/2), Vector2f(W,H));
	app.setView(view);
	app.setFramerateLimit(120);
//	app.setVerticalSyncEnabled(true);
	setWall(70*1200/2., H-70/2., 70*1200/2., 70/2.);//Bottom
	setWall(70/2., H-70*20/2., 70/2., 70*20./2.);//Left side


	setWall(70*1200.+70/2, H-70*20/2., 70/2., 70*20./2.);//Right side
	inttody(1600, 100, 32, 32, btype::box);
	b2Body *player = intplayes(200, 200, 64, btype::player);

//	world.Dump();
	bool onGnd = 0;
	auto tp1 = chrono::system_clock::now(); // Переменные для подсчета
	auto tp2 = chrono::system_clock::now();

	Font font;
	//font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
	font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
#ifdef __ANDROID__
//	font.loadFromFile("/system/fonts/Roboto-Regular.ttf");//Not working
	font.loadFromFile("fonts/FreeSans.ttf");
#endif
	char text[64];
	float force = 18000.;
	b2Vec2 pcol(0, 0);
	double ang = .0;
	b2Vec2 pos(0, 0);
	while(app.isOpen())
	{
		tp2 = chrono::system_clock::now();
				chrono::duration <double> elapsedTime = tp2 - tp1;
				tp1 = tp2;
				std::string title = "Box2d " + std::to_string((double)1/elapsedTime.count());
				app.setTitle(title);
		Event event;
		while (app.pollEvent(event))
		{

			if(event.type == Event::Closed)
				app.close();
			if(event.type == Event::LostFocus)
				while(!(event.type == Event::GainedFocus))
				{
					app.pollEvent(event);
					sleep(milliseconds(50));
				}
			else if(event.type == Event::KeyPressed)
			{
					if(event.key.code == Keyboard::Z)
						app.close();
					if(event.key.code == Keyboard::Q)
						force+=400;
					if(event.key.code == Keyboard::E)
						force-=400;
			}
		SFML::ProcessEvent(event);
		}
#ifdef __ANDROID__
		if(Touch::isDown(0)) player->ApplyForceToCenter(b2Vec2(force,0), 1);
#endif
		if(Keyboard::isKeyPressed(Keyboard::Right)) player->ApplyForceToCenter(b2Vec2(force,0), 1);
		if(Keyboard::isKeyPressed(Keyboard::Left)) player->ApplyForceToCenter(b2Vec2(-force,0), 1);
		if(Keyboard::isKeyPressed(Keyboard::Up )&& onGnd) {player->ApplyForceToCenter(b2Vec2(0,-60000), 1); onGnd = false;}
		if(Keyboard::isKeyPressed(Keyboard::Down)) {player->ApplyForceToCenter(b2Vec2(0,160000), 1);}// onGnd = false;}

		app.clear(Color::Blue);
		world.Step(1/60., 8, 3);
//		app.draw(play);

		for(b2Body *it = world.GetBodyList(); it; it = it->GetNext() )
		{
			//it->Dump();
		//world.Dump();
			//cout  <<endl;
			if(!it->GetUserData().pointer) continue;
			if(it->GetUserData().pointer == btype::box)
			{
				b2Vec2 pos = it->GetPosition();
				float ang = it->GetAngle();
				box.setPosition(pos.x*scale, pos.y*scale);
				box.setRotation(ang*deg);
				//cout <<pos.x*scale <<" " <<pos.y*scale <<endl;
				app.draw(box);
				if(pos.y*scale>(H)) world.DestroyBody(it);
			}
			else if(it->GetUserData().pointer == btype::player)
			{
				pos = it->GetPosition();
				float ang = it->GetAngle();
				play.setPosition(pos.x*scale, pos.y*scale);
				play.setRotation(ang*deg);
				//cout <<pos.x*scale <<" " <<pos.y*scale <<endl;
				view.setCenter(pos.x*scale, pos.y*scale);
				app.draw(play);
//				if(pos.y*scale>(H)) world.DestroyBody(it);
			}

		}
		for(b2Contact *c = world.GetContactList(); c; c = c->GetNext())
		{
			b2WorldManifold wm;
			c->GetWorldManifold(&wm);
			if(!c->IsTouching()) continue;
			if(c->GetFixtureB()->GetBody()->GetUserData().pointer == btype::player)
			for(int i=0; i<2; i++)
			{

//				cout <<"playe x:" <<fW(wm.points[i].x-c->GetFixtureB()->GetBody()->GetPosition().x) <<endl;
//				cout <<"playe x:" <<fW(wm.points[i].y-c->GetFixtureB()->GetBody()->GetPosition().y) <<endl;
//				printf("player x[%d]: %f \n", i, fW(wm.points[i].x));
//				printf("player y[%d]: %f \n", i, fW(wm.points[i].y));
#if DEBUG
				CircleShape pcoll(10);
				pcoll.setOrigin(5, 5);
				pcoll.setFillColor(Color::White);
				if(i)
					pcoll.setFillColor(Color::Black);
				pcoll.setPosition(Vector2f(fW(wm.points[i].x), fW(wm.points[i].y)));

				app.draw(pcoll);
#endif
				pcol = wm.points[0];

				//calculate player angle
				if((pcol.x||pcol.y) && (pcol.y>pos.y))
				{
					double a = (pos.y-pcol.y)/(pcol.x-pos.x);
					ang = abs(atan(a)*deg);
					if(ang>40.)
						onGnd = true;
				}

			}
		}

		sprintf(text, "Position x: %.2f y: %.2f\tFps: %.2f Force: %.0f Angle: %.2lf",
				play.getPosition().x, play.getPosition().y, 1/elapsedTime.count(), force, ang);
		ang = 0;
		sf::Text my(text, font);
		my.setPosition(view.getCenter().x-W/2, view.getCenter().y-H/2);
		app.draw(gr);
		app.draw(vgr);
		vgr.setPosition(70*1200, H-70*20);
		app.draw(vgr);
		vgr.setPosition(0, H-70*20);
		app.setView(view);
		my.setFillColor(Color::Black);

		SFML::Update(app, seconds(elapsedTime.count()));
		ShowDemoWindow();
		Begin("Hello");
		Button("button");
		End();
		SFML::Render(app);

		app.draw(my);
		app.display();
	}

}

#include <iostream>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdio>
#define W 1920
#define H 1080
#define scale 30.
#define fW(x) x*scale
#define tW(x) x/scale
#define deg 57.59577
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
int main()
{
	ContextSettings ctx;
	ctx.antialiasingLevel = 16;
	RenderWindow app(VideoMode(W, H), "Box2d", Style::Fullscreen, ctx);

	Texture t1, t2, t3;
	t1.loadFromFile("images/crate_43.png");
	t2.loadFromFile("images/gorilla.png");
	t3.loadFromFile("images/slice01_01.png");
	t3.setRepeated(true);
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
	char text[64];
	float force = 1800.;
	b2Vec2 pcol(0, 0);
	double ang = .0;
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
			if(event.type ==Event::Closed)
				app.close();
			else if(event.type == Event::KeyPressed)
			{
					if(event.key.code == Keyboard::Z)
						app.close();
					if(event.key.code == Keyboard::Q)
						force+=400;
					if(event.key.code == Keyboard::E)
						force-=400;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Right)) player->ApplyForceToCenter(b2Vec2(force,0), 1);
		if(Keyboard::isKeyPressed(Keyboard::Left)) player->ApplyForceToCenter(b2Vec2(-force,0), 1);
		if(Keyboard::isKeyPressed(Keyboard::Up )&& onGnd) {player->ApplyForceToCenter(b2Vec2(0,-16000), 1);}// onGnd = false;}

		app.clear(Color::Blue);
		world.Step(1/60., 8, 3);

		for(b2Contact *c = world.GetContactList(); c; c = c->GetNext())
		{
			b2WorldManifold wm;
			c->GetWorldManifold(&wm);
			if(!c->IsTouching()) continue;
			if(c->GetFixtureA()->GetBody()->GetUserData().pointer == btype::player)
			{
				printf("player x:%f ",fW(c->GetManifold()->points[1].localPoint.x));
				printf("playeer y:%f\n",fW(c->GetManifold()->points[1].localPoint.y));
				CircleShape pcol(10);
				pcol.setOrigin(5, 5);
				pcol.setFillColor(Color::Black);
				pcol.setPosition(Vector2f(fW(c->GetManifold()->points[1].localPoint.x), fW(c->GetManifold()->points[1].localPoint.y)));
//				app.draw(v, 1, Points);
				app.draw(pcol);

			}
			if(c->GetFixtureB()->GetBody()->GetUserData().pointer == btype::player)
			for(int i=0; i<2; i++)
			{
//				int i = 0;
//				printf("player x[%d]:%f ",i, fW(c->GetManifold()->points[i].localPoint.x));
//				printf("playeer y[%d]:%f\n",i, fW(c->GetManifold()->points[i].localPoint.y));
//				cout <<"playe:"<< fW(c->GetManifold()->localPoint.x) <<endl;
//				cout <<"playe:"<< fW(c->GetManifold()->localPoint.y) <<endl;
				cout <<"playe x:" <<fW(wm.points[i].x-c->GetFixtureB()->GetBody()->GetPosition().x) <<endl;
				cout <<"playe x:" <<fW(wm.points[i].y-c->GetFixtureB()->GetBody()->GetPosition().y) <<endl;
				printf("player x[%d]: %f \n", i, fW(wm.points[i].x));
				printf("player y[%d]: %f \n", i, fW(wm.points[i].y));
//				Vertex *v = new Vertex(Vector2f(fW(wm.points[i].x), fW(wm.points[i].y)));
				CircleShape pcoll(10);
				pcoll.setOrigin(5, 5);
				pcoll.setFillColor(Color::White);
				pcol = wm.points[0];
				if(i)
					pcoll.setFillColor(Color::Black);
				pcoll.setPosition(Vector2f(fW(wm.points[i].x), fW(wm.points[i].y)));
//				app.draw(v, 1, Points);
				app.draw(pcoll);
			}
//			for(b2Contact *c = world.GetContactList(); c; c = c->GetNext())
//			{
//				if(c->GetFixtureA()->GetBody()->GetUserData() ==(void*)btype::box)
//				{
//					printf("box x:%f ",fW(c->GetManifold()->points[1].localPoint.x));
//					printf("box y:%f\n",fW(c->GetManifold()->points[1].localPoint.y));

//				}
//				if(c->GetFixtureB()->GetBody()->GetUserData() ==(void*)btype::box)
//				for(int i=0; i<2; i++)
//				{
//					printf("box x[%d]:%f ",i, fW(c->GetManifold()->points[i].localPoint.x));
//					printf("box y[%d]:%f\n",i, fW(c->GetManifold()->points[i].localPoint.y));
//					cout <<"box:"<< c->GetManifold()->pointCount <<endl;
//				}
//			}
		}
		for(b2Body *it = world.GetBodyList(); it; it = it->GetNext() )
		{
			//it->Dump();
		//world.Dump();
			//cout  <<endl;
			b2Vec2 ppos = player->GetPosition();
			ppos.y += 70/scale;
			if(it->GetFixtureList()->TestPoint(ppos+b2Vec2(0, 65/scale))||
					it->GetFixtureList()->TestPoint(ppos+b2Vec2(-5/scale,66/scale))||
					it->GetFixtureList()->TestPoint(ppos+b2Vec2(5/scale,66/scale)))
				onGnd = true;
//			else onGnd = false;

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
				b2Vec2 pos = it->GetPosition();
				{
					if(pcol.x||pcol.y)
					{
						double a = (pos.y-pcol.y)/(pcol.x-pos.x);
						ang = atan(a)*deg;
						cout <<"Angle: \n" <<ang <<endl;
					}
				}
				float ang = it->GetAngle();
				play.setPosition(pos.x*scale, pos.y*scale);
				play.setRotation(ang*deg);
				//cout <<pos.x*scale <<" " <<pos.y*scale <<endl;
				view.setCenter(pos.x*scale, pos.y*scale);
				app.draw(play);
//				if(pos.y*scale>(H)) world.DestroyBody(it);
			}

		}

		sprintf(text, "Position x: %.2f y: %.2f\tFps: %.2f Force: %.0f Angle: %.2lf", play.getPosition().x, play.getPosition().y, 1/elapsedTime.count(), force, ang);
		ang = 0;
		sf::Text my(text, font);
		my.setPosition(view.getCenter().x-W/2, view.getCenter().y-H/2);
		app.draw(gr);
		app.draw(vgr);
		vgr.setPosition(70*1200, H-70*20);
		app.draw(vgr);
		vgr.setPosition(0, H-70*20);
		app.setView(view);
		app.draw(my);
		app.display();
	}

}

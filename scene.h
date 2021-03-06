#pragma once


hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable*[n + 1];
	texture* checker = new checker_texture(
		new constant_texture(vec3(0.2, 0.3, 0.1)),
		new constant_texture(vec3(0.9)));
	//list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(0.5))));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -11; a<11; a++) {
		for (int b = -11; b<11; b++) {
			float choose_mat = drand48();
			vec3 center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
			if (length(center - vec3(4, 0.2, 0)) > 0.9) {
				if (choose_mat < 0.8) {
					//list[i++] = new sphere(center, 0.2, new lambertian(mulPerElem(rndvec(), rndvec())));
					list[i++] = new moving_sphere(
						center,
						center + vec3(0, 0.5*drand48(), 0),
						0.0, 1.0, 0.2,
						new lambertian(
							new constant_texture(mulPerElem(rndvec(), rndvec()))));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(0.5*(vec3(1.0) + rndvec()), 0.5*drand48()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	//return new hitable_list(list, i);
	return new bvh_node(list, i, 0.0, 1.0);
}

hitable* refract_sphere()
{
	hitable** list = new hitable*[8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)+vec3(0.5)));
	material* yellow = new lambertian(new constant_texture(vec3(1.0, 1.0, 0.0)));
	material* blue = new lambertian(new constant_texture(vec3(0.0, 0.5f, 1.0)));
	//material* white = new lambertian(new constant_texture(vec3(0.73)));
	//material* white = new lambertian(new noise_texture(0.1));
	//material* white = new lambertian(new constant_texture(vec3(0.73)));
	material* white = new lambertian(new constant_texture(vec3(1.3)));
	material* checker = new lambertian(load_texture("assets/uvchecker.bmp"));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)+vec3(0.5)));
	//material* light = new diffuse_light(new constant_texture(vec3(15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	//list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
	//list[i++] = new flip_normals(new xz_rect(213+50, 343-50, 227+50, 332-50, 554, ));
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new texcoord(
		new flip_normals(
			new xy_rect(0, 555, 0, 555, 555, checker)
		),
		true, false, 1, 1);
	//list[i++] = new translate(
	//	new rotate_y(
	//		new box(vec3(0), vec3(165), white),
	//		-18),
	//	vec3(130,0,65));
	//material* aluminum = new metal(vec3(0.8,0.85,0.88), 0);
	//list[i++] = new translate(
	//	new rotate_y(
	//		new box(vec3(0), vec3(165,330,165), aluminum),
	//		15),
	//	vec3(265,0,295));

	list[i++] = new sphere(vec3(555/2, 555/2, 555/2), 100, new dielectric(2.417));
	list[i++] = new sphere(vec3(0, 0, 0), 10000, new sky_light());

	return new hitable_list(list, i);
}

hitable* two_spheres() {
	texture* checker = new checker_texture(
		new constant_texture(vec3(0.2, 0.3, 0.1)),
		new constant_texture(vec3(0.9)));
	hitable** list = new hitable*[2];
	list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
	return new hitable_list(list, 2);
}

hitable* two_perlin_spheres() {
	texture* noisetex = new noise_texture(4);
	hitable** list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(noisetex));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(noisetex));
	return new hitable_list(list, 2);
}

hitable* earth() {
	int nx, ny, nn;
	unsigned char* texels = stbi_load("./assets/earthmap.jpg", &nx, &ny, &nn, 0);
	material* mat = new lambertian(new image_texture(texels, nx, ny));
	return new sphere(vec3(0, 0, 0), 2, mat);
}

hitable* simple_light() {
	texture* noisetex = new noise_texture(4);
	hitable** list = new hitable*[4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(noisetex));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(noisetex));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

hitable* cornel_box() {
	hitable** list = new hitable*[8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	//list[i++] = new translate(
	//	new rotate_y(
	//		new box(vec3(0), vec3(165), white),
	//		-18),
	//	vec3(130,0,65));
	//material* aluminum = new metal(vec3(0.8,0.85,0.88), 0);
	//list[i++] = new translate(
	//	new rotate_y(
	//		new box(vec3(0), vec3(165,330,165), aluminum),
	//		15),
	//	vec3(265,0,295));

	material* glass = new dielectric(1.5);
	list[i++] = new sphere(vec3(190, 90, 190), 90, glass);
	list[i++] = new translate(
		new rotate_y(
			new box(vec3(0), vec3(165, 330, 165), white),
			15),
		vec3(265, 0, 295));

	return new hitable_list(list, i);
}

hitable* cornel_smoke() {
	hitable** list = new hitable*[8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(7)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	hitable* b1 = new translate(
		new rotate_y(
			new box(vec3(0), vec3(165), white),
			-18),
		vec3(130, 0, 65));
	hitable* b2 = new translate(
		new rotate_y(
			new box(vec3(0), vec3(165, 330, 165), white),
			15),
		vec3(265, 0, 295));

	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0)));
	return new hitable_list(list, i);
}


hitable* final_scene() {
	int nb = 20;
	hitable** list = new hitable*[30];
	hitable** boxlist = new hitable*[10000];
	hitable** boxlist2 = new hitable*[10000];
	material* white = new lambertian(new constant_texture(Vector3(0.73)));
	material* ground = new lambertian(new constant_texture(Vector3(0.48, 0.83, 0.53)));

	int b = 0;
	for (int i = 0; i<nb; ++i) {
		for (int j = 0; j<nb; ++j) {
			float w = 100;
			float x0 = -1000 + i*w;
			float z0 = -1000 + j*w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (drand48() + 0.01);
			float z1 = z0 + w;
			boxlist[b++] = new box(Vector3(x0, y0, z0), Vector3(x1, y1, z1), ground);
		}
	}

	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);

	material* light = new diffuse_light(new constant_texture(Vector3(7)));
	list[l++] = new xz_rect(123, 423, 147, 412, 554, light);

	Vector3 center(400, 400, 200);
	list[l++] = new moving_sphere(center, center + Vector3(30, 0, 0), 0, 1, 50,
		new lambertian(new constant_texture(Vector3(0.7, 0.3, 0.1))));
	list[l++] = new sphere(Vector3(260, 150, 45), 50, new dielectric(1.5));
	list[l++] = new sphere(Vector3(0, 150, 145), 50, new metal(Vector3(0.8, 0.8, 0.9), 10.0));

	hitable* boundary = new sphere(Vector3(360, 150, 145), 70, new dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2, new constant_texture(Vector3(0.2, 0.4, 0.9)));

	boundary = new sphere(Vector3(0, 0, 0), 5000, new dielectric(1.5));
	list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(Vector3(1)));

	int nx, ny, nn;
	unsigned char* texdata = stbi_load("assets/earthmap.jpg", &nx, &ny, &nn, 0);
	material* emit = new lambertian(new image_texture(texdata, nx, ny));
	list[l++] = new sphere(Vector3(400, 200, 400), 100, emit);
	texture* noisetex = new noise_texture(0.1);
	list[l++] = new sphere(Vector3(220, 280, 300), 80, new lambertian(noisetex));

	int ns = 1000;
	for (int j = 0; j<ns; ++j) {
		boxlist2[j] = new sphere(Vector3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);
	}
	list[l++] = new translate(
		new rotate_y(
			new bvh_node(boxlist2, ns, 0.0, 1.0),
			15),
		Vector3(-100, 270, 395));
	return new hitable_list(list, l);
}

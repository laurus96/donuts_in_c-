#include <cmath>
#include <cstring>
#include <iostream>


const int screen_width = 200;
const int screen_height = 50;

const float theta_spacing = 0.01;
const float phi_spacing = 0.01;

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;
const float K1 = screen_width * K2 * 1 / (8 * (R1 + R2));

void render_frame(float A, float B){
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char output[screen_width][screen_height];
    float zbuffer[screen_width][screen_height];
    memset(output, ' ', screen_width * screen_height);
    memset(zbuffer, 0, screen_width * screen_height * 4);
    for(float theta = 0; theta < 2*M_PI; theta += theta_spacing){
        float costheta = cos(theta), sintheta = sin(theta);

        for(float phi = 0; phi < 2*M_PI; phi += phi_spacing){
            float cosphi = cos(phi), sinphi = sin(phi);

            float circleX = R2 + R1*costheta;
            float circleY = R1*sintheta;

            float x = circleX*(cosB*cosphi + sinA*sinB*sinphi) - circleY*cosA*sinB;
            float y = circleX*(cosphi*sinB - cosB*sinA*sinphi) + circleY*cosA*cosB;
            float z = K2 + cosA*circleX*sinphi + circleY*sinA;
            float ooz = 1 / z;
            

            int x_proj = (int) (screen_width/2 + K1*ooz*x);
            int y_proj = (int) (screen_height/2 - K1*ooz*y);

            float L = cosphi*costheta*sinB - cosA*costheta*sinphi 
                            - sinA*sintheta + cosB*(cosA*sintheta - costheta*sinA*sinphi);

            if(L > 0){
                if(ooz > zbuffer[x_proj][y_proj]){
                    zbuffer[x_proj][y_proj] = ooz;
                    int luminance_index = L*8;
                    output[x_proj][y_proj] = ".,-~:;=!*#$@"[luminance_index];
                }
            }
        }
    }
    std::cout << "\x1b[H";
    //printf("\x1b[H");
    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            //putchar(output[i][j]);
	    std::cout << output[i][j];
    }
    //putchar("\n");
    std::cout << '\n';
  }
}


int main(){
    float A = 0, B = 0;
    for(;;){
        render_frame(A, B);
	A += 0.04;
	B += 0.02;
    }
}

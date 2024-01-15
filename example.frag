#version 330 core
 
in vec3 FragPos;  
in vec3 Normal; 
in vec3 inLightPos;
in vec3 inViewPos;
in vec3 dir;
in vec3 ex_Color; 
 
out vec4 out_Color;
 
uniform vec3 lightColor;
uniform int codCol; 
 
void main(void)
  {
    if (codCol==0) // pentru codCol==0 este aplicata iluminarea
    {
  	// Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 normala = normalize(Normal);
    vec3 lightDir = normalize(inLightPos - FragPos);
    //vec3 lightDir = normalize(dir); // cazul unei surse directionale
    float diff = max(dot(normala, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 1.0f;
    float shininess = 10.0f;

    vec3 viewDir = normalize(inViewPos - FragPos);//vector catre observator normalizat (V)
    vec3 reflectDir = reflect(-lightDir, normala); // reflexia razei de lumina (R)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  
    vec3 emission=vec3(0.0, 0.0, 0.0);
    vec3 result = emission+(ambient + diffuse + specular) * ex_Color;
	out_Color = vec4(result, 1.0f);
    }

    if (codCol==1) // pentru codCol==1 este desenata umbra
    {
        vec3 black = vec3 (0.0, 0.0, 0.0);
		out_Color = vec4 (black, 1.0);
     }
     if(codCol==2)
     {
        vec3 gray = vec3(0.2, 0.2, 0.2);
        out_Color = vec4 (gray, 1.0);
     }
     if(codCol==3)
     {
        vec3 white = vec3(1.0, 1.0, 1.0);
        out_Color = vec4 (white, 1.0);
     }
     if(codCol==4)
	 {
		vec3 red = vec3(0.7, 0.2, 0.2);
		out_Color = vec4 (red, 1.0);
	 }
     if(codCol==5) // coroana copac
     {
        vec3 green = vec3(0.5625, 0.7734375, 0.1015625);
        out_Color = vec4 (green, 1.0);
     }
     if(codCol==6)
     {
        vec3 brown = vec3(0.53515625, 0.2734375, 0.09765625);
        out_Color = vec4 (brown, 1.0);
     }
  }

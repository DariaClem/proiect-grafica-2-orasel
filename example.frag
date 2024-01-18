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
    if (codCol==1) // pentru codCol==1 este desenata umbra
    {
        vec3 black = vec3 (0.0, 0.0, 0.0);
        vec3 final = mix(vec3(0.68, 0.47, 0.2), vec3(0.0, 0.0, 0.0), 0.6);
		out_Color = vec4 (final, 1.0);
    }
    else{

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
        float specularStrength = 0.8f;
        float shininess = 100.0f;

        vec3 viewDir = normalize(inViewPos - FragPos);//vector catre observator normalizat (V)
        vec3 reflectDir = reflect(-lightDir, normala); // reflexia razei de lumina (R)
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;  
        vec3 emission=vec3(0.0, 0.0, 0.0);
        vec3 result = emission+(ambient + diffuse + specular) * ex_Color;

        // fog
        float fogMaxDist = 3000.0;
        float fogMinDist = 0.0;

        float distance = length(inViewPos - FragPos);
        float fogFactor = (fogMaxDist - distance) / (fogMaxDist - fogMinDist);
        fogFactor = clamp(fogFactor, 0.0, 1.0); // ca sa ramana valoarea intre 0 si 1

        vec3 fogColor = vec3(0.6,0.6,0.6);

        if (codCol==0) // pentru codCol==0 este aplicata iluminarea
        {
            vec3 finalResult = mix(fogColor, result, fogFactor);
	        out_Color = vec4(finalResult, 1.0f);
         }
         if(codCol==2)
         {
            vec3 gray = vec3(0.2, 0.2, 0.2);
            vec3 finalResult = mix(fogColor, gray, fogFactor);
            out_Color = vec4 (finalResult, 1.0);
         }
         if(codCol==3)
         {
            vec3 white = vec3(1.0, 1.0, 1.0);
            vec3 finalResult = mix(fogColor, white, fogFactor);
            out_Color = vec4 (finalResult, 1.0);
         }
         if(codCol==4)
	     {
		    vec3 red = vec3(0.7, 0.2, 0.2);
            vec3 finalResult = mix(fogColor, red, fogFactor);
		    out_Color = vec4 (finalResult, 1.0);
	     }
         if(codCol==5) // coroana copac
         {
            vec3 green = vec3(0.5625, 0.7734375, 0.1015625);
            vec3 finalResult = mix(fogColor, green, fogFactor);
            out_Color = vec4 (finalResult, 1.0);
         }
         if(codCol==6)
         {
            vec3 brown = vec3(0.53515625, 0.2734375, 0.09765625);
            vec3 finalResult = mix(fogColor, brown, fogFactor);
            out_Color = vec4 (finalResult, 1.0);
         }
    }
    
  }

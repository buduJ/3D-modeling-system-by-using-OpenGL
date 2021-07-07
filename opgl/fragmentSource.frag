#version 330 core									
in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;

//����
struct Material {
    vec3 ambient;//������
    sampler2D diffuse;//������
    sampler2D specular;//���淴��
    float shininess;//�����
}; 
//ƽ�й�
struct LightDirectional{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
};
//���Դ
struct LightPoint{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
	float constant;
    float linear;
    float quadratic;
};

//�۹�
struct LightSpot {
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
	float constant;
    float linear;
    float quadratic;
	float cosPhyInner ;
	float cosPhyOutter ;
};


uniform Material material;
uniform LightDirectional lightD;
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;
uniform LightSpot  lightS;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;

out vec4 FragColor;	

vec3 CalcLightDirectional(LightDirectional light,vec3 uNormal,vec3 dirToCamera)
{
  //difuse max(dot(L,N),0)
  float diffIntensity = max(dot(light.dirToLight,uNormal),0);
  vec3 diffColor = diffIntensity * light.color * texture(material.diffuse,TexCoord).rgb;

  //specular pow(max(dot(R,cam),0),shininess)
  vec3 R = normalize(reflect(-light.dirToLight,uNormal));
  float specIntensity = pow(max(dot( R ,dirToCamera),0),material.shininess);
  vec3 speColor = specIntensity * light.color * texture(material.specular,TexCoord).rgb;


  vec3 result = diffColor + speColor;
  return result;

}


vec3 CalcLightPoint(LightPoint light,vec3 uNormal,vec3 dirToCamera)
{ //attenuation
  float dist = length(light.pos - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
  
  //diffuse
  float diffIntensity = max(dot(normalize(light.pos - FragPos),uNormal),0) * attenuation;
  vec3 diffColor = diffIntensity * light.color * texture(material.diffuse,TexCoord).rgb;

  //specular
  vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal));
  float specIntensity = pow(max(dot( R ,dirToCamera),0),material.shininess * attenuation);
  vec3 speColor = specIntensity * light.color * texture(material.specular,TexCoord).rgb;

  vec3 result = diffColor + speColor;
  return result;

}


vec3 CalcLightSpot(LightSpot light,vec3 uNormal,vec3 dirToCamera)
{
  //attenuation
  float dist = length(light.pos - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
  float spotRatio;
  float CosTheta = dot ( normalize(FragPos - light.pos), - light.dirToLight);

  if (CosTheta > light.cosPhyInner) 
      {       
	  spotRatio = 1.0f;
      // inside
       }
     else if(CosTheta > light.cosPhyOutter) 
       {
	  spotRatio = (CosTheta - light.cosPhyInner) / ( light.cosPhyInner - lightS.cosPhyOutter);
	   //middle
	   }
	   else{
	  spotRatio = 0  ;
	   //outside
	   }

  //diffuse
  float diffIntensity = max(dot(normalize(light.pos - FragPos),uNormal),0) * attenuation * spotRatio;
  vec3 diffColor = diffIntensity * light.color * texture(material.diffuse,TexCoord).rgb;

  //specular
  vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal));
  float specIntensity = pow(max(dot( R ,dirToCamera),0),material.shininess) * attenuation * spotRatio;
  vec3 speColor = specIntensity * light.color * texture(material.specular,TexCoord).rgb;

  vec3 result = diffColor + speColor;
  return result;
}



void main()
{	
    vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos - FragPos);
	finalResult += 2 * CalcLightDirectional(lightD,uNormal,dirToCamera);//ƽ�й�
	finalResult += 2 * CalcLightPoint(lightP0,uNormal,dirToCamera);//���Դ0
	finalResult += 2 * CalcLightPoint(lightP1,uNormal,dirToCamera);//���Դ1
	finalResult += 2 * CalcLightPoint(lightP2,uNormal,dirToCamera);//���Դ2
	finalResult += 2 * CalcLightPoint(lightP3,uNormal,dirToCamera);//���Դ3
	FragColor = vec4(finalResult, 1.0);//���
};	                


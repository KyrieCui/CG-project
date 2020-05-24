#version 330 core
in vec3 oNormal;//光照法线
in vec2 TexCoords;

in vec3 oFragPos;//光照世界位置
uniform vec3 lightPos;//光源位置
uniform vec3 viewPos;//眼睛的位置
uniform sampler2D image;
uniform sampler2D image1;
uniform vec3 spriteColor;
out vec4 color;

//光照漫反射
float LightDiffuse(vec3 _normal,vec3 _fragpos, vec3 _lightpos)
{
   vec3 norm = normalize(_normal);
   vec3 lightDir = normalize(_lightpos - _fragpos);
   float diff = max(dot(norm, lightDir), 0.0);
   return diff;
}

//光照镜面反射
float LightSpecular(vec3 _normal,vec3 _fragpos, vec3 _lightpos,vec3 _viewpos)
{
   float specularStrength = 0.5;//镜面强度(Specular Intensity)变量
   vec3 viewDir = normalize(_viewpos - _fragpos);
   vec3 lightDir = normalize(_lightpos - _fragpos);
   vec3 reflectDir = reflect(-lightDir, _normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   return  spec;

}
//点光源 衰减
float DotLightAttenuation(vec3 _fragpos, vec3 _lightpos)
{
//下面这个公式根据片段距光源的距离计算了衰减值，之后我们会将它乘以光的强度向量：
 //Fatt=1.0/(Kc+Kl∗d+Kq∗d2)  在这里d 代表了片段距光源的距离。接下来为了计算衰减值，我们定义3个（可配置的）项：常数项Kc、一次项Kl和二次项Kq。
 float KC = 1.0;
 float KL = 0.045;
 float KQ = 0.0075;
 float  d = length(_lightpos - _fragpos);
 float attenuation = 1.0 / (KC + KL * d + KQ * (d * d));

 return attenuation;
}

//聚光
//LightDir：从片段指向光源的向量。
//SpotDir：聚光所指向的方向。
//Phiϕ：指定了聚光半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
//Thetaθ：LightDir向量和SpotDir向量之间的夹角。在聚光内部的话θ值应该比ϕ值小。
float SpotLight(vec3 _fragpos, vec3 _lightpos,vec3 _lightdir)
{
 float Phi = 0.7;//Phiϕ：指定了聚光半径的切光角
 vec3 lightDir = normalize(_lightpos - _fragpos);
 float theta = dot(lightDir, normalize(- _lightdir));

 float par =  clamp((theta -Phi) / (0.85-0.8), 0.0, 1.0);
 
 return par;
}


void main()
{
   vec3 lightpos  =  vec3(110,100,180);//光源位置
   float diff     = LightDiffuse(oNormal,oFragPos,lightpos);//漫反射
   float specular = LightSpecular(oNormal,oFragPos,lightpos,viewPos);//镜面反射
   //衰减因子
   float _dotattenuation = DotLightAttenuation(oFragPos,lightpos);
   //探照灯
   vec3 _lightdir = vec3(0,-1,0);
   float spotlight =  SpotLight(oFragPos,lightpos,_lightdir);
   //总光照因子
   float lightPar =   (diff * 0.5 + specular * 0.5)*spotlight ;
   color =  vec4(vec3(texture(image, TexCoords)*((lightPar + 0.2) )),0.5);
}
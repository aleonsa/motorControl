datos = load('Datos2.txt');
tiempo= linspace(0,27.54,5508);
u=12*datos(:, 1)/1024;
u=u';
im=datos(:, 2)';
for i=1:2:1010
    im(i)=im(i+1);
end 
velocidad=datos(:, 3);
plot(u)
figure()
plot(im)
figure()
plot(velocidad)
estado=[im;velocidad'];
y =estado(:,2:end)';
U=[estado(:,1:end-1);u(:,1:end-1)]';
theta=(pinv(U)*y)';
A=theta(:,1:end-1);
B=(theta(:,end));
%%
imM=out.Salida.Data(1:end,1);
vM=out.Salida.Data(1:end,2);
subplot(2,1,1)
plot(im)
hold on
plot(imM)
legend('Real','Estimada')
title('Corriente')
subplot(2,1,2)
plot(velocidad)
 hold on
 plot(vM)
 legend('Real','Estimada')
title('Velocidad')
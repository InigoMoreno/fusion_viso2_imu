
addpath(genpath('matlab_functions'))
figure()
system('cmake-build-debug\test_fusion_viso2_imu.exe 3 0 config.yaml');
plot_data
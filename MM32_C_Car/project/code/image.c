#include "zf_common_headfile.h"
#include "image.h"

uint8 reference_point = 0;
uint8 white_max_point = 0;             //动态白点最大值
uint8 white_min_point = 0;             //动态白点最小值
uint8 remote_distance[IMAGE_W] = {0};   //最长白列
uint8 reference_col = 0;
uint8 reference_contrast_ratio = 20; //参考对比度
uint16 reference_line[IMAGE_H] = {0};      // 存储参考列
uint16 left_edge_line[IMAGE_H] = {0};      // 存储左边线
uint16 right_edge_line[IMAGE_H] = {0};      // 存储右边线
uint8 user_image[IMAGE_H][IMAGE_W];  //存储图像

void get_image(void){
	memcpy(user_image,mt9v03x_image,IMAGE_H*IMAGE_W);
}




void get_reference_point(const uint8 image[][IMAGE_W]){
	uint16 temp = 0;                 //用于保存统计点总数量
	uint32 temp1 = 0;               //用于保存统计点总数值
	uint16 temp_j1 = (IMAGE_W-REFERENCE_COL)/2;    
	uint16 temp_j2 = (IMAGE_W+REFERENCE_COL)/2;	
	
	temp = REFERENCE_ROW * REFERENCE_COL;
	for(int i = IMAGE_H-REFERENCE_ROW-1;i <= IMAGE_H-1;i++){
		for(int j = temp_j1;j <= temp_j2;j++){
			temp1 += image[i][j];				//统计点求和
		}
	}
	reference_point = (uint8)(temp1/temp);
	white_max_point = (uint8)((int32)reference_point * WHITEMAXMUL/10);
	white_min_point = (uint8)((int32)reference_point * WHITEMINMUL/10);
	if(white_min_point < BLACKPOINT)
		white_min_point = BLACKPOINT;
}

void search_reference_col(const uint8 image[][IMAGE_W]){
	int col,row;
	int16 temp1 = 0,temp2 = 0,temp3 = 0;           //当前点 对比点 对比度
	for(col = 0;col < IMAGE_W;col++){
		remote_distance[col] = IMAGE_H-1;
	}
	for(col = 0;col < IMAGE_W;col+=CONTRASTOFFSET){                  //跳行，隔两行算一次
		for	(row = IMAGE_H-1;row > STOPROW+CONTRASTOFFSET;row-=CONTRASTOFFSET){    //跳列，隔两列判断一次
			if (row - CONTRASTOFFSET < 0) break;
			temp1 = image[row][col];
			temp2 = image[row-CONTRASTOFFSET][col];
			
			if(temp2 > white_max_point){           //对比点是白点
				continue;
			}
			if(temp1 < white_min_point){           //当前点是黑点
				remote_distance[col] = (uint8)row;
				break;
 			}
			
			temp3 = (temp1 - temp2)*200/(temp1 + temp2);
			
			if(temp3 >reference_contrast_ratio || row == STOPROW){   //计算对比度
				remote_distance[col] = (uint8)row;
				break;
			}
		}
	}
	
	for(col = 0;col < IMAGE_W;col+=CONTRASTOFFSET){    //寻找最长白列
		if(remote_distance[reference_col] > remote_distance[col])
			reference_col = col;
	}
	for(row = 0;row < IMAGE_H;row++){
		reference_line[row] =reference_col;
	}
}


void search_line(const uint8 image[][IMAGE_W]){
    uint8 row_max = IMAGE_H - 1;                        // 行最大值
    uint8 row_min = STOPROW;                                    // 行最小值
    uint8 col_max = IMAGE_W - CONTRASTOFFSET-1;           // 列最大值
    uint8 col_min = CONTRASTOFFSET;                    // 列最小值
    int16 leftstartcol  = reference_col;                // 搜线左起始列
    int16 rightstartcol = reference_col;                // 搜线右起始列
    int16 leftendcol    = col_min;                           // 搜线左终止列
    int16 rightendcol   = col_max;                   // 搜线右终止列
    uint8 search_time   = 0;                                    // 单点搜线次数
    uint8 temp1 = 0, temp2 = 0;                                 // 临时变量  用于存储图像数 
    int  temp3 = 0;                                             // 临时变量  用于存储对比度
    int  leftstop = 0, rightstop = 0, stoppoint = 0;            // 搜线自锁变量

    int col, row;

    for(row = row_max; row >= row_min; row --){                //边线归零到两边
		left_edge_line[row]  = col_min - CONTRASTOFFSET;
        right_edge_line[row] = col_max + CONTRASTOFFSET;
    }
	
	for(row = row_max;row >= row_min;row--){                   
		if(!leftstop){
			search_time = 2;
			do{
				if(search_time == 1){                         //第一次没搜到
					leftstartcol = reference_col;
					leftendcol = col_min;
				}
				
				search_time--;
				
				for(col = leftstartcol; col >= leftendcol ;col--){
					if(col - CONTRASTOFFSET <0)
						continue;
					temp1 = image[row][col];
					temp2 = image[row][col - CONTRASTOFFSET];
				
					if(temp1 < white_min_point && col == leftstartcol && leftstartcol == reference_col){
						//判断参考列是否为黑点，若是则这行不再搜索左边线
						leftstop = 1;
						for(stoppoint = row;stoppoint >= 0;stoppoint--){
							left_edge_line[stoppoint ] = col_min;		//将自锁行以上的边线归零
						}
						search_time = 0;
						break;
					}
					
					if(temp1 < white_min_point){           //当前点为黑点
						left_edge_line[row] = col;
						break;
					}
					if(temp2 > white_max_point){           //参考点为白点
						continue;
					}
					temp3 = (temp1 - temp2)*200/(temp1 + temp2); //计算对比度
					
					if(temp3 >reference_contrast_ratio || col == col_min){   
						left_edge_line[row] = func_limit_ab(col - CONTRASTOFFSET, 0, IMAGE_W-1);		//保存左边界
						//下一行先在搜线半径内搜线
						leftstartcol = (uint8)func_limit_ab(col+SEARCHRANGE,col,col_max);  
						leftendcol = (uint8)func_limit_ab(col-SEARCHRANGE,col_min,col);
						search_time = 0;
						break;
					}
				}
			}while(search_time);
		}
		
		
		if(!rightstop){
			search_time = 2;
			do{
				if(search_time == 1){                         //第一次没搜到
					rightstartcol = reference_col;
					rightendcol = col_max;
				}
				
				search_time--;
				
				for(col = rightstartcol; col <= rightendcol ;col++){
					if(col - CONTRASTOFFSET <0)
						continue;
					temp1 = image[row][col];
					temp2 = image[row][col + CONTRASTOFFSET];
				
					if(temp1 < white_min_point && col == rightstartcol && rightstartcol == reference_col){
						//判断参考列是否为黑点，若是则这行不再搜索右边线
						rightstop = 1;
						for(stoppoint = row;stoppoint >= 0;stoppoint--){
							right_edge_line[stoppoint ] = col_max;		//将自锁行以上的边线归零
						}
						search_time = 0;
						break;
					}
					
					if(temp1 < white_min_point){           //当前点为黑点
						right_edge_line[row] = col;
						break;
					}
					if(temp2 > white_max_point){           //参考点为白点
						continue;
					}
					temp3 = (temp1 - temp2)*200/(temp1 + temp2); //计算对比度
					
					if(temp3 >reference_contrast_ratio || col == col_max){   
						right_edge_line[row] = func_limit_ab(col + CONTRASTOFFSET, 0, IMAGE_W-1);		//保存右边界
						//下一行先在搜线半径内搜线
						rightstartcol = (uint8)func_limit_ab(col-SEARCHRANGE,col_min,col);  
						rightendcol = (uint8)func_limit_ab(col+SEARCHRANGE,col,col_max);
						search_time = 0;
						break;
					}
				}
			}while(search_time);
		}
	}
}



void image_display_edge_line(const uint8 image[][IMAGE_W],uint16 display_width,uint16 display_height){
	ips200_displayimage03x((const uint8 *)image, display_width, display_height);
	
	for(uint16 i = 0;i<IMAGE_H;i++){
		ips200_draw_point(left_edge_line[i],i,RGB565_RED);
		ips200_draw_point(right_edge_line[i],i,RGB565_BLUE);
		ips200_draw_point(reference_line[i],i,RGB565_YELLOW);
	}
}

void image_core(uint16 display_width,uint16 display_height){
	get_image();
	reference_point = 0; white_max_point = 0;white_min_point = 0;reference_col = 0;
	
	get_reference_point(user_image);
	search_reference_col(user_image);
	search_line(user_image);
	
	image_display_edge_line(user_image,display_width,display_height);
}

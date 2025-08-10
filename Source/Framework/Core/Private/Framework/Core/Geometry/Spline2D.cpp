//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Spline2D.h>
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

    // ヘルパー関数：制御点を取得（範囲外は端点を使用）
    Vec2 Spline2D::getControlPoint(s32 index) const {
        if (points.empty()) return Vec2::Zero;
        
        if (index < 0) {
            // 範囲外の場合、端点を使用
            if (closed && points.size() > 2) {
                return points[points.size() + index];
            } else {
                return points[0];
            }
        } else if (index >= static_cast<s32>(points.size())) {
            // 範囲外の場合、端点を使用
            if (closed && points.size() > 2) {
                return points[index % points.size()];
            } else {
                return points[points.size() - 1];
            }
        }
        
        return points[index];
    }


    //! @brief  カーブの長さを計算
    //! 
    //! @params resolution いくつの直線で近似した長さを計算するか
    f32 Spline2D::length(s32 resolution)const {
        return length(0.0f, 1.0f, resolution);
    }


    //! @brief  [t0,t1]のカーブの長さを計算
    //! 
    //! @details t0,t1が[0,1]以外の場合は[0,1]にクランプされた値が計算されます。
    //! @params t0 開始位置[0,t1]
    //! @params t1 終了位置[t1,1]
    //! @params resolution いくつの直線で近似した長さを計算するか
    f32 Spline2D::length(f32 t0, f32 t1, s32 resolution)const {
        if (points.empty()) return 0.0f;
        if (points.size() == 1) return 0.0f;
        
        // t0, t1を[0,1]にクランプおよび順序を保証
        t0 = Math::Clamp01(t0);
        t1 = Math::Clamp01(t1);
        if (t0 > t1) {
            f32 temp = t0;
            t0 = t1;
            t1 = temp;
        }
        
        if (Math::IsNearEquals(t0, t1)) return 0.0f;
        
        // 線形補間（ポイント数が2個の場合）
        if (points.size() == 2) {
            const Vec2 start = Vec2::Lerp(points[0], points[1], t0);
            const Vec2 end = Vec2::Lerp(points[0], points[1], t1);
            return Vec2::Dist(start, end);
        }
        
        // 数値積分で長さを近似計算
        f32 totalLength = 0.0f;
        const f32 step = (t1 - t0) / static_cast<f32>(resolution);
        
        Vec2 prevPos = position(t0);
        for (s32 i = 1; i <= resolution; ++i) {
            const f32 t = t0 + step * static_cast<f32>(i);
            const Vec2 currentPos = position(t);
            totalLength += Vec2::Dist(prevPos, currentPos);
            prevPos = currentPos;
        }
        
        return totalLength;
    }


    //! @brief  座標を計算
    //! 
    //! @params t 時間[0,1]
    Vec2 Spline2D::position(f32 t)const {
        if (points.empty()) return Vec2::Zero;
        if (points.size() == 1) return points[0];
        
        // tを[0,1]にクランプ
        t = Math::Clamp01(t);
        
        // 線形補間（ポイント数が2個の場合）
        if (points.size() == 2) {
            return Vec2::Lerp(points[0], points[1], t);
        }
        
        // Catmull-Romスプライン補間
        const f32 segmentCount = static_cast<f32>(points.size() - 1);
        const f32 scaledT = t * segmentCount;
        const s32 segmentIndex = Math::Min(static_cast<s32>(scaledT), static_cast<s32>(points.size() - 2));
        const f32 localT = scaledT - static_cast<f32>(segmentIndex);
        
        // 4つの制御点を取得
        const Vec2 p0 = getControlPoint(segmentIndex - 1);
        const Vec2 p1 = points[segmentIndex];
        const Vec2 p2 = points[segmentIndex + 1];
        const Vec2 p3 = getControlPoint(segmentIndex + 2);
        
        // Catmull-Rom補間式
        const f32 t2 = localT * localT;
        const f32 t3 = t2 * localT;
        
        const f32 c0 = -0.5f * t3 + t2 - 0.5f * localT;
        const f32 c1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
        const f32 c2 = -1.5f * t3 + 2.0f * t2 + 0.5f * localT;
        const f32 c3 = 0.5f * t3 - 0.5f * t2;
        
        return p0 * c0 + p1 * c1 + p2 * c2 + p3 * c3;
    }


    //! @brief  速度を計算
    //! 
    //! @params t 時間[0,1]
    Vec2 Spline2D::velocity(f32 t)const {
        if (points.empty() || points.size() == 1) return Vec2::Zero;
        
        // tを[0,1]にクランプ
        t = Math::Clamp01(t);
        
        // 線形補間（ポイント数が2個の場合）
        if (points.size() == 2) {
            return points[1] - points[0];
        }
        
        // Catmull-Romスプライン速度計算（1次導関数）
        const f32 segmentCount = static_cast<f32>(points.size() - 1);
        const f32 scaledT = t * segmentCount;
        const s32 segmentIndex = Math::Min(static_cast<s32>(scaledT), static_cast<s32>(points.size() - 2));
        const f32 localT = scaledT - static_cast<f32>(segmentIndex);
        
        // 4つの制御点を取得
        const Vec2 p0 = getControlPoint(segmentIndex - 1);
        const Vec2 p1 = points[segmentIndex];
        const Vec2 p2 = points[segmentIndex + 1];
        const Vec2 p3 = getControlPoint(segmentIndex + 2);
        
        // Catmull-Rom補間の1次導関数
        const f32 t2 = localT * localT;
        
        const f32 c0 = -1.5f * t2 + 2.0f * localT - 0.5f;
        const f32 c1 = 4.5f * t2 - 5.0f * localT;
        const f32 c2 = -4.5f * t2 + 4.0f * localT + 0.5f;
        const f32 c3 = 1.5f * t2 - localT;
        
        return (p0 * c0 + p1 * c1 + p2 * c2 + p3 * c3) * segmentCount;
    }


    //! @brief  加速度を計算
    //! 
    //! @params t 時間[0,1]
    Vec2 Spline2D::acceleration(f32 t)const {
        if (points.empty() || points.size() <= 2) return Vec2::Zero;
        
        // tを[0,1]にクランプ
        t = Math::Clamp01(t);
        
        // Catmull-Romスプライン加速度計算（2次導関数）
        const f32 segmentCount = static_cast<f32>(points.size() - 1);
        const f32 scaledT = t * segmentCount;
        const s32 segmentIndex = Math::Min(static_cast<s32>(scaledT), static_cast<s32>(points.size() - 2));
        const f32 localT = scaledT - static_cast<f32>(segmentIndex);
        
        // 4つの制御点を取得
        const Vec2 p0 = getControlPoint(segmentIndex - 1);
        const Vec2 p1 = points[segmentIndex];
        const Vec2 p2 = points[segmentIndex + 1];
        const Vec2 p3 = getControlPoint(segmentIndex + 2);
        
        // Catmull-Rom補間の2次導関数
        const f32 c0 = -3.0f * localT + 2.0f;
        const f32 c1 = 9.0f * localT - 5.0f;
        const f32 c2 = -9.0f * localT + 4.0f;
        const f32 c3 = 3.0f * localT - 1.0f;
        
        return (p0 * c0 + p1 * c1 + p2 * c2 + p3 * c3) * (segmentCount * segmentCount);
    }


    //! @brief  曲率を計算
    //! 
    //! @params t 時間[0,1]
    f32 Spline2D::curvature(f32 t)const {
        if (points.empty() || points.size() <= 2) return 0.0f;
        
        const Vec2 vel = velocity(t);
        const Vec2 acc = acceleration(t);
        
        const f32 velMag = vel.sqrLength();
        if (velMag < Math::TOLERANCE) return 0.0f;
        
        // 曲率 = |v × a| / |v|^3
        const f32 cross = Vec2::Cross(vel, acc);
        const f32 velMag32 = velMag * Math::Sqrt(velMag);
        
        return Math::Abs(cross) / velMag32;
    }

}